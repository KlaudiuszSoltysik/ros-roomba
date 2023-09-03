from launch import LaunchDescription
from launch.actions import ExecuteProcess
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

from launch_ros.actions import Node
import xacro
import os


def generate_launch_description():
    pkg_name = 'roomba'
    pkg_share = FindPackageShare(pkg_name)
    # xacro_subpath = 'rviz/roomba.urdf.xacro'

    base_node = Node(
        package='roomba',
        executable='base',
        name='base')

    camera_translate_node = Node(
        package='roomba',
        executable='camera_translate',
        name='camera_translate')

    # gazebo = ExecuteProcess(
    #     cmd=[
    #         'ign gazebo',
    #         PathJoinSubstitution([
    #             pkg_share,
    #             'gazebo',
    #             'roomba_world.sdf'])],
    #     shell=True)
    
    # rviz = Node(
    #     package='rviz2',
    #     executable='rviz2',
    #     arguments=['-d', PathJoinSubstitution([
    #         pkg_share,
    #         'rviz',
    #         'config.rviz'])])
    
    
    # xacro_file = os.path.join(pkg_share, xacro_subpath)
    # robot_description = xacro.process_file(xacro_file).toxml()
    
    # robot_state_publisher_node = Node(
    #     package='robot_state_publisher',
    #     executable='robot_state_publisher',
    #     output='screen',
    #     parameters=[{'robot_description': robot_description,
    #     'use_sim_time': True}]
    # )

    return LaunchDescription([
        base_node,
        camera_translate_node,
        # gazebo,
        # rviz,
    ])