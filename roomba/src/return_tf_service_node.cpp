#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"


class ReturnTf : public rclcpp::Node {
  public:
    ReturnTf() : Node("return_tf") {
      // Prompt user about succesfully created node
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The return tf service node ran successfully");
    }

  private:
    // rclcpp::Service<custom_interfaces::srv::GoTo>::SharedPtr srv;
};


int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ReturnTf>());
  rclcpp::shutdown();
  return 0;
}
