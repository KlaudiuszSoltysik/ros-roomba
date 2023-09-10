#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"


class CleanRoom : public rclcpp::Node {
  public:
    CleanRoom() : Node("clean_room") {
      // Prompt user about succesfully created node
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The clean room node ran successfully");
    }
};


int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CleanRoom>());
  rclcpp::shutdown();
  return 0;
}
