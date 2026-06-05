import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image, CameraInfo
from cv_bridge import CvBridge
import message_filters

import cv2
import json
import math
import numpy as np
import os
from rclpy.qos import qos_profile_sensor_data

# for transforming to base link
from geometry_msgs.msg import PointStamped
import tf2_ros
import tf2_geometry_msgs # for point stamped messages

# for semantic detections
from semantic_msgs.msg import SemanticDetection
from semantic_msgs.msg import SemanticDetectionArray

# for vlm backend
from obj_detection.vlm_backends import GeminiBackend, OllamaBackend
from PIL import Image as PILImage

# Note: Before running this file, remember to export "GOOGLE_API_KEY" in the terminal you are running this node

class VLMStreaming(Node):
    def __init__(self):
        super().__init__('vlm_streaming_node')

        # i am trying to make this modular
        # so instead of hardcoding make parameters such that you deliberately and explicitly pass the topic
        # names to the vlm streaming detector
        # first declare the parameters
        self.declare_parameter('color_topic', '')
        self.declare_parameter('depth_topic', '')
        self.declare_parameter('camera_info_topic', '')
        self.declare_parameter('camera_optical_frame', '')
        # then get the parameters
        self.color_topic = self.get_parameter('color_topic').value
        self.depth_topic = self.get_parameter('depth_topic').value
        self.camera_info_topic = self.get_parameter('camera_info_topic').value
        self.camera_optical_frame = self.get_parameter('camera_optical_frame').value

        self.declare_parameter('fx', 0.0)
        self.declare_parameter('fy', 0.0)
        self.declare_parameter('cx', 0.0)
        self.declare_parameter('cy', 0.0)
        self.declare_parameter('throttle_hz', 1.0)   # VLM call rate limit

        self.fx = self.get_parameter('fx').value
        self.fy = self.get_parameter('fy').value
        self.cx = self.get_parameter('cx').value
        self.cy = self.get_parameter('cy').value

        self.throttle_period = 1.0 / self.get_parameter('throttle_hz').value
        self.intrinsics_ready = False
        self._last_vlm_call = 0.0

        # tf to base link
        self.tf_buffer   = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self)

        self.bridge = CvBridge()

        # load landmark labels from JSON to constrain VLM output vocabulary
        self.declare_parameter('landmark_json_path', '')
        landmark_json_path = self.get_parameter('landmark_json_path').value
        labels = self._load_landmark_labels(landmark_json_path)
        self.get_logger().info(f"Landmark labels for VLM: {labels}")

        # declare and get backend param
        self.declare_parameter('vlm_backend', 'ollama')
        backend_name = self.get_parameter('vlm_backend').value
        if backend_name == 'gemini':
            api_key = os.environ.get("GOOGLE_API_KEY")
            if not api_key:
                self.get_logger().error("GOOGLE_API_KEY not set!")
                raise RuntimeError("Missing GOOGLE_API_KEY")
            self.backend = GeminiBackend(api_key=api_key, labels=labels)
        else:
            self.backend = OllamaBackend(labels=labels)

        # subscribe to the CameraInfo topic
        self.create_subscription(CameraInfo, self.camera_info_topic, self.camera_info_cb, qos_profile_sensor_data)

        # passively subscribe to rgb and depth and ensuree that they are properly sync
        color_sub = message_filters.Subscriber(self, Image, self.color_topic, qos_profile=qos_profile_sensor_data)
        depth_sub = message_filters.Subscriber(self, Image, self.depth_topic, qos_profile=qos_profile_sensor_data)

        self.ts = message_filters.ApproximateTimeSynchronizer(
            [color_sub, depth_sub], queue_size=5, slop=0.05
        )

        # publisher to publish vlm semantic detections
        self.vlm_publisher = self.create_publisher(SemanticDetectionArray, 'vlm/semantic_detections', 10)

        # debug image publisher for bounding box visualization
        self.debug_img_pub = self.create_publisher(Image, 'vlm/debug_image', 10)

        self.ts.registerCallback(self.image_cb)

        # becauyse trhe vlm keeps detecting objects while there isnt in the scene, we introduce this variance
        # which determines how the depth in the pixels differs in relation to one another
        # if high variance, that yeah, there is an object that is closer to the image than isnt
        # if low variance, this means that it is a wall since the depth for each pixel is the same
        self.declare_parameter('min_depth_std', 0.2)
        self.min_depth_std = self.get_parameter('min_depth_std').value

        self.get_logger().info("Initialized VLM Streamer Node. Waiting for camera feed...")

    def image_cb(self, color_msg: Image, depth_msg: Image):
        if not self.intrinsics_ready:
            self.get_logger().warn("Waiting for intrinsics...", throttle_duration_sec=2.0)
            return

        # throttle vlm calls as they are expensive
        now = self.get_clock().now().nanoseconds * 1e-9
        if (now - self._last_vlm_call) < self.throttle_period:
            return
        self._last_vlm_call = now

        color_cv = self.bridge.imgmsg_to_cv2(color_msg, desired_encoding='bgr8')
        depth_cv = self.bridge.imgmsg_to_cv2(depth_msg, desired_encoding='passthrough')

        pil_img = PILImage.fromarray(cv2.cvtColor(color_cv, cv2.COLOR_BGR2RGB))
        vlm_results = self.detect_all_objects(pil_img, color_cv.shape)

        # publish debug image with bounding boxes
        self._publish_debug_image(color_cv, vlm_results, color_msg.header)

        # prepare semantic detection array message
        msg = SemanticDetectionArray()
        msg.header = color_msg.header

        if not vlm_results:
            # Publish an explicit empty array so the MCL node knows there are no
            # detections this cycle and skips the semantic weight update.
            self.vlm_publisher.publish(msg)
            self.get_logger().info("No objects detected.", throttle_duration_sec=2.0)
            return

        table_results = []
        for det in vlm_results:
            u, v = det["u"], det["v"]
            Z = self.sample_depth(depth_cv, u, v, patch=5)
            if Z is None:
                continue

            # check the depth variance to check if an object is there
            # get the bouinding box pixel coords
            u1, v1, u2, v2 = det["box_px"]
            # get the sliced depth image and get the valid depth values
            region = depth_cv[v1:v2, u1:u2].astype(np.float32)
            valid  = region[np.isfinite(region) & (region > 0.01)]
            depth_std = float(np.std(valid)) if valid.size > 0 else 0.0
            # debug message to check rthe standard deviation
            self.get_logger().info(                                                                                                                                                                                                                                                                  
                f"{det['label']}: depth_std={depth_std:.4f}"
            )       
            # check if there are no valid pixels or it is below this threshold
            if valid.size == 0 or depth_std < self.min_depth_std:
                self.get_logger().info(                                                                                                                                                                                                                                                                  
                    f"Rejected '{det['label']}' — depth_std={depth_std:.3f} < {self.min_depth_std}"                                                                                                                                                                                                              
                )
                continue # skip because it is just walls

            X_cam = (u - self.cx) * Z / self.fx
            Y_cam = (v - self.cy) * Z / self.fy
            
            result = self.transform_to_base_link(X_cam, Y_cam, Z, color_msg.header.stamp)
            if result is None:
                # Fallback to camera-frame bearing/range if TF fails
                bearing, range_m = self.get_bearing_and_range(u, v, Z)
            else:
                bearing, range_m = result

            detection = SemanticDetection()
            detection.label = det["label"]
            detection.bearing = bearing # in radian mode
            detection.range = range_m
            detection.confidence = float(det.get("confidence", 1.0))
            
            msg.detections.append(detection)

            table_results.append({
                "label":   det["label"],
                "pixel":   (u, v),
                "depth_m": round(Z, 2),
                "range_m": round(range_m, 2),
                "bearing_deg": round(math.degrees(bearing), 1),
                "confidence": float(det.get("confidence", 1.0))
            })

        self.vlm_publisher.publish(msg)

        if table_results:
            self.get_logger().info(
                "\n" + self._format_table(table_results)
            )

    def detect_all_objects(self, pil_img, img_shape) -> list[dict]:
        return self.backend.detect(pil_img=pil_img, img_shape=img_shape)

    @staticmethod
    def _load_landmark_labels(json_path: str) -> list[str]:
        if not json_path:
            return []
        try:
            with open(json_path, 'r') as f:
                data = json.load(f)
            # JSON is {"landmarks": [...]} or a bare list
            entries = data["landmarks"] if isinstance(data, dict) else data
            return list({entry["label"] for entry in entries if "label" in entry})
        except Exception as e:
            print(f"[VLMStreaming] Failed to load landmark JSON: {e}")
            return []


    def camera_info_cb(self, msg: CameraInfo):
        if self.intrinsics_ready:
            return
        if self.fx == 0:
            self.fx = msg.k[0]
            self.fy = msg.k[4]
            self.cx = msg.k[2]
            self.cy = msg.k[5]
        self.intrinsics_ready = True
        self.get_logger().info(
            f"Intrinsics loaded: fx={self.fx:.1f} fy={self.fy:.1f} "
            f"cx={self.cx:.1f} cy={self.cy:.1f}"
        )


    def get_bearing_and_range(self, u: int, v: int, Z: float) -> tuple[float, float]:
        """
        Back-project pixel (u,v) + depth Z into camera frame,
        then compute bearing (horizontal) and range (ground-plane distance).

        Camera frame convention (ROS/OpenCV):
            +X → right, +Y → down, +Z → forward (into scene)

        Returns:
            bearing (float): angle in radians, positive = object to the RIGHT
            range   (float): horizontal distance in metres (sqrt(X² + Z²))
        """
        X = (u - self.cx) * Z / self.fx # lateral offset
        # Y = (v - self.cy) * Z / self.fy # vertical offset, not used here
        
        bearing = math.atan2(X, Z)
        range_m = math.sqrt(X**2 + Z**2)

        return bearing, range_m

    def transform_to_base_link(self, X_cam: float, Y_cam: float, Z_cam: float, stamp) -> tuple[float, float] | None:
        """
        Transform a point from kinect_camera_optical frame to base_link frame,
        then return bearing and range relative to the robot centre.
        """
        point_cam = PointStamped()
        point_cam.header.frame_id = self.camera_optical_frame
        point_cam.header.stamp    = stamp
        # In optical frame: X=right, Y=down, Z=forward
        point_cam.point.x = X_cam
        point_cam.point.y = Y_cam
        point_cam.point.z = Z_cam

        try:
            point_base = self.tf_buffer.transform(
                point_cam, "base_link", timeout=rclpy.duration.Duration(seconds=0.1)
            )
            x = point_base.point.x   # forward in base_link
            y = point_base.point.y   # left in base_link

            bearing = math.atan2(y, x)          # ROS convention: +y = left
            range_m = math.sqrt(x**2 + y**2)
            return bearing, range_m

        except Exception as e:
            self.get_logger().warn(f"TF transform failed: {e}")
            return None

    def sample_depth(self, depth_cv: np.ndarray, u: int, v: int, patch : int = 5) -> float | None:
        h, w = depth_cv.shape[:2]
        half = patch // 2
        u0, u1 = max(0, u - half), min(w, u + half + 1)
        v0, v1 = max(0, v - half), min(h, v + half + 1)
        region = depth_cv[v0:v1, u0:u1].astype(np.float32)
        valid  = region[np.isfinite(region) & (region > 0.01)]
        return float(np.median(valid)) if valid.size > 0 else None


    @staticmethod
    def _format_table(results: list[dict]) -> str:
        header = f"{'Label':<20} {'Range(m)':>8} {'Bearing(°)':>10} {'Pixel':>12} {'Depth(m)':>9}"
        sep    = "-" * len(header)
        rows   = [
            f"{r['label']:<20} {r['range_m']:>8.2f} {r['bearing_deg']:>10.1f} "
            f"{str(r['pixel']):>12} {r['depth_m']:>9.2f}"
            for r in sorted(results, key=lambda x: x['range_m'])
        ]
        return "\n".join([sep, header, sep] + rows + [sep])
    
    def _publish_debug_image(self, img_cv: np.ndarray, vlm_results: list[dict], header):
        """
        Draws bounding boxes and labels on the image and publishes it.
        """
        # Save resources by not processing if nothing is subscribed to the debug topic
        if self.debug_img_pub.get_subscription_count() == 0:
            return

        # Make a copy to avoid mutating the original image array
        debug_img = img_cv.copy()

        for det in vlm_results:
            label = det.get("label", "Unknown")
            u = int(det.get("u", 0))
            v = int(det.get("v", 0))
            confidence = float(det.get("confidence", 1.0))
            text = f"{label} ({confidence:.2f})"

            # Check if bounding box data exists in your backend's output.
            # Adjust the "bbox" key to match what GeminiBackend/OllamaBackend actually returns.
            bbox = det.get("bbox") 
            
            if bbox and len(bbox) == 4:
                # Assuming format is [xmin, ymin, xmax, ymax]
                xmin, ymin, xmax, ymax = map(int, bbox)
                
                # Draw bounding box
                cv2.rectangle(debug_img, (xmin, ymin), (xmax, ymax), (0, 255, 0), 2)
                
                # Draw center point
                cv2.circle(debug_img, (u, v), 4, (0, 0, 255), -1)
                
                # Draw label text above the bounding box
                text_y = max(ymin - 10, 10)
                cv2.putText(debug_img, text, (xmin, text_y),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
            else:
                # Fallback: If no bounding box is provided, just draw the center point (u, v)
                cv2.circle(debug_img, (u, v), 5, (0, 0, 255), -1)
                cv2.putText(debug_img, text, (u + 10, v),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)

        # Convert back to ROS Image message and publish
        try:
            debug_msg = self.bridge.cv2_to_imgmsg(debug_img, encoding='bgr8')
            debug_msg.header = header
            self.debug_img_pub.publish(debug_msg)
        except Exception as e:
            self.get_logger().error(f"Failed to publish debug image: {e}")

def main(args=None):
    rclpy.init(args=args)
    node = VLMStreaming()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()

