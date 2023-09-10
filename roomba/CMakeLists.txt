cmake_minimum_required(VERSION 3.8)
project(roomba)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(OpenCV REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(custom_interfaces REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(base src/base_node.cpp)
add_executable(camera_translate src/camera_translate_node.cpp)
add_executable(robot_tf_publisher src/robot_tf_publisher_node.cpp)
add_executable(return_tf_service src/return_tf_service_node.cpp)
add_executable(map_room src/map_room_node.cpp)
add_executable(go_to_service src/go_to_service_node.cpp)
add_executable(clean_room src/clean_room_node.cpp)

ament_target_dependencies(base rclcpp)
ament_target_dependencies(camera_translate rclcpp std_msgs sensor_msgs OpenCV)
ament_target_dependencies(robot_tf_publisher rclcpp geometry_msgs nav_msgs tf2_ros)
ament_target_dependencies(return_tf_service rclcpp)
ament_target_dependencies(map_room rclcpp)
ament_target_dependencies(go_to_service rclcpp custom_interfaces geometry_msgs)
ament_target_dependencies(clean_room rclcpp)

install(TARGETS
  base
  camera_translate
  robot_tf_publisher
  return_tf_service
  map_room
  go_to_service
  clean_room
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
