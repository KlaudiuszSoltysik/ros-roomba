#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"


class Base : public rclcpp::Node {
  public:
    Base() : Node("base") {
      const char* base_bash_path = "/home/klaudiusz/Documents/ros_roomba/src/roomba/bash_scripts/base.bash";

      // Run a bash script that acts as launch file
      if(system(base_bash_path) == 0) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The bash script ran successfully");
      } else {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "The bash script failed");
        return;
      }

      // Prompt user about succesfully created node
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The base node ran successfully");
    }
};


int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Base>());
  rclcpp::shutdown();
  return 0;
}
