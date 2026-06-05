import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from std_srvs.srv import Trigger
from cv_bridge import CvBridge
import cv2

# import vlm libraies
from google import genai
from google.genai import types
from PIL import Image as PILImage

import json
import os
from rclpy.qos import qos_profile_sensor_data

using_bcr_bot = False

# Note: Before running this file, remember to export "GOOGLE_API_KEY" in the terminal you are running this node

class VLMService(Node):
    def __init__(self):
        super().__init__('vlm_service_node')

        # subscribe to raw image topic
        self.latest_cv_image = None
        if using_bcr_bot:
            self.image_sub = self.create_subscription(Image, '/bcr_bot/kinect_camera/image_raw', self.image_callback, qos_profile_sensor_data)
        else:
            self.image_sub = self.create_subscription(Image, '/camera/image_raw', self.image_callback, qos_profile_sensor_data)

        # publish what vlm detects
        self.image_pub = self.create_publisher(Image, '/vlm/detections', 10)
        self.bridge = CvBridge()

        # a service to trigger vlm api call
        self.srv = self.create_service(Trigger, '/trigger_vlm', self.vlm_trigger_callback)

        # init the gemini client
        api_key = os.environ.get("GOOGLE_API_KEY")
        if not api_key:
            self.get_logger().error("GOOGLE_API_KEY not found! Please run: export GOOGLE_API_KEY='your_key'")
        
        self.client = genai.Client(api_key=api_key)
        self.get_logger().info("VLM Detector Node Initialized and Ready.")

    def image_callback(self, msg):
        try:
            self.latest_cv_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding="bgr8")
        except Exception as e:
            self.get_logger().error(f'Image buffering error: {e}')

    def vlm_trigger_callback(self, request, response):
        # return failure if camera did not get a camera feed
        if self.latest_cv_image is None:
            response.success = False
            response.message = "No camera frame received yet."
            return response

        self.get_logger().info("I am sending the latest image to Gemini Client for processing!")

        # convert bgr to rgb
        rgb_image = cv2.cvtColor(self.latest_cv_image, cv2.COLOR_BGR2RGB)
        pil_image = PILImage.fromarray(rgb_image)
        height, width, _ = self.latest_cv_image.shape

        prompt = """
            Analyze this scene for robotics navigation. 
            Return a JSON array of objects. Each object must have:
            1. "class": The category (e.g., "door", "fire_extinguisher", "trash_can")
            2. "confidence": A float between 0 and 1
            3. "box_2d": [ymin, xmin, ymax, xmax] (normalized 0-1000)
            4. "label": A brief description (e.g., "red fire extinguisher")
            
            Format strictly as JSON:
            [{"class": "type", "confidence": 0.95, "box_2d": [ymin, xmin, ymax, xmax], "label": "description"}]
        """

        try:
            # call gemini
            # temperature affects how creative the answer is, so 1.0 is very creative and 0.0 is very deterministic and precise
            api_response = self.client.models.generate_content(
                model = 'gemini-2.5-flash',
                contents = [pil_image, prompt],
                config = types.GenerateContentConfig(
                    response_mime_type = "application/json",
                    temperature=0.0 
                )
            )

            # load the output of the vlm in json format
            detections = json.loads(api_response.text)
            annotated_image = self.latest_cv_image.copy()

            # parse and draw bounding boxes
            for det in detections:
                # extract the class
                obj_class = det.get('class', 'unknown')
                confidence = det.get('confidence', 0.0)
                box = det.get('box_2d', [0.0, 0.0, 0.0, 0.0])

                # bounding box dimensions
                ymin = int((box[0] / 1000) * height)
                xmin = int((box[1] / 1000) * width)
                ymax = int((box[2] / 1000) * height)
                xmax = int((box[3] / 1000) * width)

                # calculating center of the bounding box in x and y
                center_x = (xmin + xmax) // 2
                center_y = (ymin + ymax) // 2

                display_text = f"{obj_class}: {confidence:.2f}"

                # draw the bounding box and make it green and 2 pixels thick
                cv2.rectangle(annotated_image, (xmin, ymin), (xmax, ymax), (0, 255, 0), 2)

                # Draw Label Background (for readability)
                (text_w, text_h), _ = cv2.getTextSize(display_text, cv2.FONT_HERSHEY_SIMPLEX, 0.5, 1)
                cv2.rectangle(annotated_image, (xmin, ymin - 20), (xmin + text_w, ymin), (0, 255, 0), -1)
                
                # Draw Text
                cv2.putText(annotated_image, display_text, (xmin, ymin - 5), 
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 1)

                self.get_logger().info(
                    f"Detected [{obj_class}] | Confidence: {confidence:.2f} | bbx: {center_x} | bby: {center_y}"
                )

            # publish results to rviz
            img_msg = self.bridge.cv2_to_imgmsg(annotated_image, encoding="bgr8")
            self.image_pub.publish(img_msg)

            response.success = True
            response.message = f"Detected {len(detections)} semantic anchors."
            self.get_logger().info(response.message)

        except Exception as e:
            self.get_logger().error(f'VLM Callback failed: {e}')
            response.success = False
            response.message = str(e)

        return response


def main(args=None):
    rclpy.init(args=args)
    node = VLMService()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()