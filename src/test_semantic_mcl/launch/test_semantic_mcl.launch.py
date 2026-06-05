from launch import LaunchDescription
from ament_index_python.packages import get_package_share_directory
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from launch.actions import SetEnvironmentVariable, AppendEnvironmentVariable, DeclareLaunchArgument
import os
import yaml
from launch.actions import OpaqueFunction
from launch.substitutions import LaunchConfiguration

def launch_setup(context, *args, **kwargs):
    # converts substitutiobn object into a string value
    robot_name = LaunchConfiguration('rebot').perform(context)
    

def generate_launch_description():
    # spawn the gazebo world with my levbel 6 wor
    # zxcasedqwe123ld
    bcr_bot_path = get_package_share_directory('bcr_bot')
    gazebo_share = get_package_share_directory('gazebo_ros')

    # use the bcr bot
    robot_name = DeclareLaunchArgument('robot', default_value='bcr_bot')

    set_gazebo_resource_path = SetEnvironmentVariable(
      name='GAZEBO_RESOURCE_PATH',                                                                                                                                                                                                                                                         
      value="/usr/share/gazebo-11:" + os.path.join(bcr_bot_path, "worlds")                                                                                                                                                                                                               
    )                                                                                                                                                                                                                                                                                        
   
    set_gazebo_model_path = AppendEnvironmentVariable(                                                                                                                                                                                                                                       
        name='GAZEBO_MODEL_PATH',                                                                                                                                                                                                                                                          
        value=os.path.join(bcr_bot_path, "models")                                                                                                                                                                                                                                           
    )                                                                                                                                                                                                                                                                                      

    gazebo = IncludeLaunchDescription(                                                                                                                                                                                                                                                       
        PythonLaunchDescriptionSource(os.path.join(gazebo_share, "launch", "gazebo.launch.py")),
        launch_arguments={"world": os.path.join(                                                                                                                                                                                                                                             
            get_package_share_directory("test_semantic_mcl"), "worlds", "level6_v2"                                                                                                                                                                                                        
        )}.items()                                                                                                                                                                                                                                                                           
    )
                                                                                                                                                                                                                                                                                            
    spawn_bcr_bot = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(bcr_bot_path, "launch", "bcr_bot_gazebo_spawn.launch.py")),
        launch_arguments={
            "position_x": "-5.0", # just beside that two doors
            "position_y": "-4.0", # just beside that two doors
            # "position_x": "5.5",
            # "position_y": "5.5",
            "orientation_yaw": "1.57",
            "robot_namespace": "",
            "front_camera_enabled" : "true",
        }.items()
    )

    # map server node
    map_server = Node(                                                                                                                                                                                                                                                                       
      package="nav2_map_server",
      executable="map_server",  
      parameters=[{"yaml_filename": os.path.join(
          get_package_share_directory("test_semantic_mcl"), "map", "map.yaml"
      )}]                                                                                                                                                                                                                                                                                  
  )      
                                                                                                                                                                                                                                                                                           
    lifecycle_manager = Node(
      package="nav2_lifecycle_manager",                                                                                                                                                                                                                                                    
      executable="lifecycle_manager",  
      parameters=[{"autostart": True, "node_names": ["map_server"]}]
  )      

    # Single source of truth: landmark_json_path lives only in semantic_mcl_params.yaml.
    # All other nodes read the value from there at launch time.
    params_yaml = os.path.join(
        get_package_share_directory("semantic_mcl"), "config", "semantic_mcl_params.yaml"
    )
    with open(params_yaml, 'r') as f:
        _p = yaml.safe_load(f)
    landmark_json = _p["semantic_mcl_node"]["ros__parameters"]["landmark_json_path"]

    # semantic mcl node — loads all its parameters from the yaml file directly
    semantic_mcl_node = Node(
        package="semantic_mcl",
        executable="semantic_mcl_node_exec",
        parameters=[params_yaml],
    )

    # landmark visualizer node — path comes from the same yaml, no duplication
    landmark_visualizer_node = Node(
        package="test_semantic_mcl",
        executable="landmark_visualizer",
        parameters=[{"landmark_json_path": landmark_json}],
    )

    ld = LaunchDescription()

    # add the actions
    ld.add_action(set_gazebo_resource_path)
    ld.add_action(set_gazebo_model_path)
    ld.add_action(gazebo)
    ld.add_action(spawn_bcr_bot)
    ld.add_action(map_server)
    ld.add_action(lifecycle_manager)
    ld.add_action(semantic_mcl_node)
    ld.add_action(landmark_visualizer_node)

    return ld