import json

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, DurabilityPolicy, ReliabilityPolicy
from visualization_msgs.msg import Marker, MarkerArray


# Map landmark labels to RGB colours (0.0–1.0 each)
_LABEL_COLOURS = {
    "fire_hydrant": (1.0, 0.0, 0.0),   # red
    "trash_can":    (0.0, 0.8, 0.0),   # green
    "door":         (0.0, 0.4, 1.0),   # blue
}
_DEFAULT_COLOUR = (1.0, 1.0, 1.0)      # white for unknown labels

_SPHERE_RADIUS  = 0.3   # metres
_SPHERE_HEIGHT  = 0.0   # z of sphere centre
_TEXT_HEIGHT    = 0.8   # z of text label above ground


class LandmarkVisualizer(Node):
    def __init__(self):
        super().__init__('landmark_visualizer')

        self.declare_parameter('landmark_json_path', '')
        json_path = self.get_parameter('landmark_json_path').value

        landmarks = self._load_landmarks(json_path)
        if not landmarks:
            self.get_logger().error(
                f"No landmarks loaded from '{json_path}'. "
                "Pass --ros-args -p landmark_json_path:=<path>"
            )

        # Transient-local so RViz receives the markers even if it subscribes late
        qos = QoSProfile(
            depth=1,
            durability=DurabilityPolicy.TRANSIENT_LOCAL,
            reliability=ReliabilityPolicy.RELIABLE,
        )
        self._pub = self.create_publisher(MarkerArray, 'landmark_markers', qos)
        self._pub.publish(self._build_marker_array(landmarks))
        self.get_logger().info(
            f"Published {len(landmarks)} landmark marker(s) to /landmark_markers"
        )

    # ------------------------------------------------------------------
    def _load_landmarks(self, json_path: str) -> list[dict]:
        if not json_path:
            return []
        try:
            with open(json_path, 'r') as f:
                data = json.load(f)
            entries = data['landmarks'] if isinstance(data, dict) else data
            return [e for e in entries if 'label' in e and 'x' in e and 'y' in e]
        except Exception as e:
            self.get_logger().error(f"Failed to load landmarks: {e}")
            return []

    def _build_marker_array(self, landmarks: list[dict]) -> MarkerArray:
        array = MarkerArray()
        for idx, lm in enumerate(landmarks):
            label = lm['label']
            x, y  = float(lm['x']), float(lm['y'])
            r, g, b = _LABEL_COLOURS.get(label, _DEFAULT_COLOUR)

            # Sphere marker at landmark position
            sphere = Marker()
            sphere.header.frame_id = 'map'
            sphere.ns   = 'landmark_spheres'
            sphere.id   = idx
            sphere.type = Marker.SPHERE
            sphere.action = Marker.ADD
            sphere.pose.position.x = x
            sphere.pose.position.y = y
            sphere.pose.position.z = _SPHERE_HEIGHT
            sphere.pose.orientation.w = 1.0
            sphere.scale.x = _SPHERE_RADIUS * 2
            sphere.scale.y = _SPHERE_RADIUS * 2
            sphere.scale.z = _SPHERE_RADIUS * 2
            sphere.color.r = r
            sphere.color.g = g
            sphere.color.b = b
            sphere.color.a = 0.85

            # Text label floating above the sphere
            text = Marker()
            text.header.frame_id = 'map'
            text.ns   = 'landmark_labels'
            text.id   = idx
            text.type = Marker.TEXT_VIEW_FACING
            text.action = Marker.ADD
            text.pose.position.x = x
            text.pose.position.y = y
            text.pose.position.z = _TEXT_HEIGHT
            text.pose.orientation.w = 1.0
            text.scale.z = 0.25   # TEXT_VIEW_FACING only uses scale.z (font height)
            text.color.r = r
            text.color.g = g
            text.color.b = b
            text.color.a = 1.0
            text.text = label

            array.markers.append(sphere)
            array.markers.append(text)

        return array


def main(args=None):
    rclpy.init(args=args)
    node = LandmarkVisualizer()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
