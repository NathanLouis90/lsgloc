import math
import numpy as np

import rclpy
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import Image, CameraInfo
from cv_bridge import CvBridge
import message_filters

from geometry_msgs.msg import PointStamped
import tf2_ros
import tf2_geometry_msgs

from ultralytics import YOLO

from semantic_msgs.msg import SemanticDetection, SemanticDetectionArray

using_bcr_bot = True

# DEPTH_TOPIC = "/bcr_bot/kinect_camera/depth/image_raw" if using_bcr_bot else "/camera/depth/image_raw"
# COLOR_TOPIC = "/bcr_bot/kinect_camera/image_raw"       if using_bcr_bot else "/camera/color/image_raw"
# INFO_TOPIC  = "/bcr_bot/kinect_camera/camera_info"     if using_bcr_bot else "/camera/depth/camera_info"

DEPTH_TOPIC = "/kinect_camera/depth/image_raw" if using_bcr_bot else "/camera/depth/image_raw"
COLOR_TOPIC = "/kinect_camera/image_raw"       if using_bcr_bot else "/camera/color/image_raw"
INFO_TOPIC  = "/kinect_camera/camera_info"     if using_bcr_bot else "/camera/depth/camera_info"


class YoloDetectorNode(Node):
    def __init__(self):
        super().__init__('yolo_node')

        self.declare_parameter('fx', 0.0)
        self.declare_parameter('fy', 0.0)
        self.declare_parameter('cx', 0.0)
        self.declare_parameter('cy', 0.0)

        self.fx = self.get_parameter('fx').value
        self.fy = self.get_parameter('fy').value
        self.cx = self.get_parameter('cx').value
        self.cy = self.get_parameter('cy').value

        self.intrinsics_ready = False

        self.tf_buffer = tf2_ros.Buffer()
        self.tf_listener = tf2_ros.TransformListener(self.tf_buffer, self)

        self.bridge = CvBridge()

        self.model = YOLO('yolov8x.pt')

        self.create_subscription(CameraInfo, INFO_TOPIC, self.camera_info_cb, qos_profile_sensor_data)

        color_sub = message_filters.Subscriber(self, Image, COLOR_TOPIC, qos_profile=qos_profile_sensor_data)
        depth_sub = message_filters.Subscriber(self, Image, DEPTH_TOPIC, qos_profile=qos_profile_sensor_data)

        self.ts = message_filters.ApproximateTimeSynchronizer(
            [color_sub, depth_sub], queue_size=5, slop=0.05
        )
        self.ts.registerCallback(self.image_cb)

        self.semantic_pub = self.create_publisher(SemanticDetectionArray, 'yolo/semantic_detections', 10)
        self.visual_pub = self.create_publisher(Image, 'yolo/annotated', 10)

        self.get_logger().info('YOLO Semantic Detector Initialized. Waiting for camera feed...')

    def image_cb(self, color_msg: Image, depth_msg: Image):
        if not self.intrinsics_ready:
            self.get_logger().warn("Waiting for intrinsics...", throttle_duration_sec=2.0)
            return

        color_cv = self.bridge.imgmsg_to_cv2(color_msg, desired_encoding='bgr8')
        depth_cv = self.bridge.imgmsg_to_cv2(depth_msg, desired_encoding='passthrough')

        results = self.model(color_cv, verbose=False)
        boxes = results[0].boxes

        # publish annotated image
        annotated = results[0].plot()
        self.visual_pub.publish(self.bridge.cv2_to_imgmsg(annotated, encoding='bgr8'))

        if boxes is None or len(boxes) == 0:
            return

        msg = SemanticDetectionArray()
        msg.header = color_msg.header

        table_results = []
        for i in range(len(boxes)):
            x1, y1, x2, y2 = boxes.xyxy[i].tolist()
            u = int((x1 + x2) / 2)
            v = int((y1 + y2) / 2)
            label = self.model.names[int(boxes.cls[i].item())].replace(' ', '_')
            confidence = float(boxes.conf[i].item())

            Z = self.sample_depth(depth_cv, u, v, patch=5)
            if Z is None:
                continue

            X_cam = (u - self.cx) * Z / self.fx
            Y_cam = (v - self.cy) * Z / self.fy

            result = self.transform_to_base_link(X_cam, Y_cam, Z, color_msg.header.stamp)
            if result is None:
                bearing, range_m = self.get_bearing_and_range(u, v, Z)
            else:
                bearing, range_m = result

            detection = SemanticDetection()
            detection.label = label
            detection.bearing = bearing
            detection.range = range_m
            detection.confidence = confidence

            msg.detections.append(detection)

            table_results.append({
                "label": label,
                "pixel": (u, v),
                "depth_m": round(Z, 2),
                "range_m": round(range_m, 2),
                "bearing_deg": round(math.degrees(bearing), 1),
                "confidence": confidence
            })   

        self.semantic_pub.publish(msg)

        if table_results:
            self.get_logger().info("\n" + self._format_table(table_results))

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
        X = (u - self.cx) * Z / self.fx
        bearing = math.atan2(X, Z)
        range_m = math.sqrt(X**2 + Z**2)
        return bearing, range_m

    def transform_to_base_link(self, X_cam: float, Y_cam: float, Z_cam: float, stamp) -> tuple[float, float] | None:
        point_cam = PointStamped()
        point_cam.header.frame_id = "kinect_camera_optical"
        point_cam.header.stamp = stamp
        point_cam.point.x = X_cam
        point_cam.point.y = Y_cam
        point_cam.point.z = Z_cam

        try:
            point_base = self.tf_buffer.transform(
                point_cam, "base_link", timeout=rclpy.duration.Duration(seconds=0.1)
            )
            x = point_base.point.x
            y = point_base.point.y
            bearing = math.atan2(y, x)
            range_m = math.sqrt(x**2 + y**2)
            return bearing, range_m
        except Exception as e:
            self.get_logger().warn(f"TF transform failed: {e}")
            return None

    def sample_depth(self, depth_cv: np.ndarray, u: int, v: int, patch: int = 5) -> float | None:
        h, w = depth_cv.shape[:2]
        half = patch // 2
        u0, u1 = max(0, u - half), min(w, u + half + 1)
        v0, v1 = max(0, v - half), min(h, v + half + 1)
        region = depth_cv[v0:v1, u0:u1].astype(np.float32)
        valid = region[np.isfinite(region) & (region > 0.01)]
        return float(np.median(valid)) if valid.size > 0 else None

    @staticmethod
    def _format_table(results: list[dict]) -> str:
        header = f"{'Label':<20} {'Range(m)':>8} {'Bearing(°)':>10} {'Pixel':>12} {'Depth(m)':>9} {'Conf':>6}"
        sep = "-" * len(header)
        rows = [
            f"{r['label']:<20} {r['range_m']:>8.2f} {r['bearing_deg']:>10.1f} "
            f"{str(r['pixel']):>12} {r['depth_m']:>9.2f} {r['confidence']:>6.2f}"
            for r in sorted(results, key=lambda x: x['range_m'])
        ]
        return "\n".join([sep, header, sep] + rows + [sep])


def main(args=None):
    rclpy.init(args=args)
    node = YoloDetectorNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
