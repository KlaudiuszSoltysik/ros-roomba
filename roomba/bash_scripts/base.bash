#!/bin/bash
gnome-terminal -- bash -c "
# Launch the Gazebo simulation
ign gazebo /home/klaudiusz/Documents/ros_roomba/src/roomba/gazebo/roomba_world.sdf &

# Launch RViz2 from config file
rviz2 -d /home/klaudiusz/Documents/ros_roomba/src/roomba/rviz/config.rviz &

sleep 1

# Run the Gazebo bridges
ros2 run ros_gz_bridge parameter_bridge /imu@sensor_msgs/msg/Imu[ignition.msgs.IMU &
ros2 run ros_gz_bridge parameter_bridge /lidar@sensor_msgs/msg/LaserScan[ignition.msgs.LaserScan &
ros2 run ros_gz_bridge parameter_bridge /cmd_vel@geometry_msgs/msg/Twist]ignition.msgs.Twist &

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
exec bash"
