import launch
import launch_ros.actions

def generate_launch_description():
    return launch.LaunchDescription([
        launch_ros.actions.Node(
            package='roomba',
            executable='base',
            name='base'),
        # launch_ros.actions.Node(
        #     package='roomba',
        #     executable='map',
        #     name='map'),
        ])