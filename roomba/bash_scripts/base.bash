#!/bin/bash
gnome-terminal -- bash -c '
# Launch the Gazebo simulation
ign gazebo /home/klaudiusz/Documents/ros_roomba/src/roomba/gazebo/roomba_world.sdf &

sleep 1

# Run the Gazebo bridges
ros2 run ros_gz_bridge parameter_bridge /imu@sensor_msgs/msg/Imu[ignition.msgs.IMU &
ros2 run ros_gz_bridge parameter_bridge /lidar@sensor_msgs/msg/LaserScan[ignition.msgs.LaserScan &
ros2 run ros_gz_bridge parameter_bridge /world/roomba_world/model/roomba/joint_state@sensor_msgs/msg/JointState[ignition.msgs.Model --ros-args -r /world/roomba_world/model/roomba/joint_state:=/joint_states &
ros2 run ros_gz_bridge parameter_bridge /camera@sensor_msgs/msg/Image[ignition.msgs.Image &

ros2 run ros_gz_bridge parameter_bridge /cmd_vel@geometry_msgs/msg/Twist]ignition.msgs.Twist &
ros2 run robot_state_publisher robot_state_publisher --ros-args -p robot_description:="$( xacro /home/klaudiusz/Documents/ros_roomba/src/roomba/rviz/roomba.urdf.xacro )" &

sleep 1

# Launch RViz2 from config file
rviz2 -d /home/klaudiusz/Documents/ros_roomba/src/roomba/rviz/config.rviz &

# Change directory
cd /home/klaudiusz/Documents/ros_roomba/src/roomba/webpage/

# Run HTTP server on port 7000
python -m http.server 7000 &

sleep 1

# Run the web bridge
ros2 launch rosbridge_server rosbridge_websocket_launch.xml &

sleep 1

# Open Chrome on port 7000
google-chrome http://0.0.0.0:7000/ &

# Start a new bash shell
exec bash'
