#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/go_to.hpp"
#include "geometry_msgs/msg/twist.hpp"


class GoTo : public rclcpp::Node {
  public:
    GoTo() : Node("go_to") {
      // Create go to service server 
      srv = create_service<custom_interfaces::srv::GoTo>("go_to", std::bind(&GoTo::goToCb, this, std::placeholders::_1, std::placeholders::_2));

      // Prompt user about succesfully created node
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The go to service node ran successfully");
    }

  private:
    rclcpp::Service<custom_interfaces::srv::GoTo>::SharedPtr srv;

    void goToCb(const std::shared_ptr<custom_interfaces::srv::GoTo::Request> request, std::shared_ptr<custom_interfaces::srv::GoTo::Response> response) {
      

      response->result = true;
    }
};


int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GoTo>());
  rclcpp::shutdown();
  return 0;
}
