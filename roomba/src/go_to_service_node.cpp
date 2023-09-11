#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "custom_interfaces/srv/go_to.hpp"
#include "custom_interfaces/srv/return_tf.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"


class GoTo : public rclcpp::Node {
  public:
    GoTo() : Node("go_to") {
      // Create go to service server 
      srv = create_service<custom_interfaces::srv::GoTo>("go_to", std::bind(&GoTo::goToCb, this, std::placeholders::_1, std::placeholders::_2));
      
      // Create return tf service client
      cli = create_client<custom_interfaces::srv::ReturnTf>("return_tf");

      while(!cli->wait_for_service(std::chrono::seconds(1))) {
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service \"return_tf\" not available, waiting again...");
      }

      // Create tf buffer and listener
      tf_buffer = std::make_unique<tf2_ros::Buffer>(get_clock());
      tf_listener = std::make_shared<tf2_ros::TransformListener>(*tf_buffer);

      // Prompt user about succesfully created node
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The go to service node ran successfully");
    }

  private:
    rclcpp::Service<custom_interfaces::srv::GoTo>::SharedPtr srv;
    rclcpp::Client<custom_interfaces::srv::ReturnTf>::SharedPtr cli;
    std::unique_ptr<tf2_ros::Buffer> tf_buffer;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener;
    geometry_msgs::msg::TransformStamped starting_tf;
    geometry_msgs::msg::TransformStamped ending_tf;

    void goToCb(const std::shared_ptr<custom_interfaces::srv::GoTo::Request> go_to_req, std::shared_ptr<custom_interfaces::srv::GoTo::Response> go_to_resp) {
      starting_tf = tf_buffer->lookupTransform("world", "roomba", tf2::TimePointZero);

      auto return_tf_req = std::make_shared<custom_interfaces::srv::ReturnTf::Request>();

      return_tf_req->x = go_to_req->x;
      return_tf_req->y = go_to_req->y;

      auto result_future = cli->async_send_request(return_tf_req);
      
      // RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "%f", result->tf.transform.translation.x);

      go_to_resp->result = true;
    }
};


int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GoTo>());
  rclcpp::shutdown();
  return 0;
}
