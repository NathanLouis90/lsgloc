from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='teleop_twist_keyboard',
            executable='teleop_twist_keyboard',
            name='teleop_bcr_node',
            output='screen',
            prefix='xterm -e',
            remappings=[
                ('/cmd_vel', '/bcr_bot/cmd_vel')
            ]
        )
    ])
