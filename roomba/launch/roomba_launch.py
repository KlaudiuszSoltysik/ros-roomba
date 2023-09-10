from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    base_node = Node(
        package='roomba',
        executable='base',
        name='base')

    camera_translate_node = Node(
        package='roomba',
        executable='camera_translate',
        name='camera_translate')
    
    robot_tf_publisher_node = Node(
        package='roomba',
        executable='robot_tf_publisher',
        name='robot_tf_publisher')
    
    return_tf_service_node = Node(
        package='roomba',
        executable='return_tf_service',
        name='return_tf_service')
    
    map_room_node = Node(
        package='roomba',
        executable='map_room',
        name='map_room')
    
    go_to_node = Node(
        package='roomba',
        executable='go_to_service',
        name='go_to_service')
    
    clean_room_node = Node(
        package='roomba',
        executable='clean_room',
        name='clean_room')

    return LaunchDescription([
        base_node,
        camera_translate_node,
        robot_tf_publisher_node,
        return_tf_service_node,
        map_room_node,
        go_to_node,
        clean_room_node,
    ])