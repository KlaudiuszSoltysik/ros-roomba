#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <cmath>

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

      // Create cmd vel publisher
      pub = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);

      // Prompt user about succesfully created node
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The go to service node ran successfully");
    }

  private:
    rclcpp::Service<custom_interfaces::srv::GoTo>::SharedPtr srv;
    rclcpp::Client<custom_interfaces::srv::ReturnTf>::SharedPtr cli;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub;
    std::unique_ptr<tf2_ros::Buffer> tf_buffer;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener;

    void goToCb(const std::shared_ptr<custom_interfaces::srv::GoTo::Request> go_to_req, std::shared_ptr<custom_interfaces::srv::GoTo::Response> go_to_resp) {
      auto return_tf_req = std::make_shared<custom_interfaces::srv::ReturnTf::Request>();

      return_tf_req->x = go_to_req->x;
      return_tf_req->y = go_to_req->y;

      auto response_received_callback = [this](rclcpp::Client<custom_interfaces::srv::ReturnTf>::SharedFuture future) {
        auto ending_tf = future.get();

        geometry_msgs::msg::TransformStamped current_tf = tf_buffer->lookupTransform("world", "base_link", tf2::TimePointZero);
        tf2::Quaternion q(
          current_tf.transform.rotation.x,
          current_tf.transform.rotation.y,
          current_tf.transform.rotation.z,
          current_tf.transform.rotation.w);
        tf2::Matrix3x3 m(q);
        double roll, pitch, yaw_before;
        m.getRPY(roll, pitch, yaw_before);

        while(true) {
          geometry_msgs::msg::TransformStamped current_tf = tf_buffer->lookupTransform("world", "base_link", tf2::TimePointZero);
          tf2::Quaternion q(
            current_tf.transform.rotation.x,
            current_tf.transform.rotation.y,
            current_tf.transform.rotation.z,
            current_tf.transform.rotation.w);
          tf2::Matrix3x3 m(q);
          double roll, pitch, yaw_current;
          m.getRPY(roll, pitch, yaw_current);

          double delta_theta = atan2(ending_tf->tf.transform.translation.y - current_tf.transform.translation.y, ending_tf->tf.transform.translation.x - current_tf.transform.translation.x) - (yaw_current - yaw_before);
          
          while(delta_theta >= 2 * M_PI) {
            delta_theta -= 2 * M_PI;
          }

          while(delta_theta <= -2 * M_PI) {
            delta_theta += 2 * M_PI;
          }

          geometry_msgs::msg::Twist msg;

          msg.angular.z = (delta_theta > 0) ? 0.5 : -0.5;

          pub->publish(msg);
          
          if(abs(delta_theta) < 0.1) {
            msg.angular.z = 0;
            pub->publish(msg);
            break;
          }

          rclcpp::sleep_for(std::chrono::milliseconds(10));
        }

        double delta_d = std::numeric_limits<double>::infinity();
        double old_delta_d;

        while(true) {
          geometry_msgs::msg::TransformStamped current_tf = tf_buffer->lookupTransform("world", "base_link", tf2::TimePointZero);

          old_delta_d = delta_d;

          delta_d = sqrt(pow(ending_tf->tf.transform.translation.x - current_tf.transform.translation.x, 2) + pow(ending_tf->tf.transform.translation.y - current_tf.transform.translation.y, 2));

          geometry_msgs::msg::Twist msg;

          msg.linear.x = 1.0;

          pub->publish(msg);
          
          if(delta_d > old_delta_d) {
            msg.linear.x = 0;
            pub->publish(msg);
            break;
          }

          rclcpp::sleep_for(std::chrono::milliseconds(10));
        }
      };

      auto result_future = cli->async_send_request(return_tf_req, response_received_callback);

      go_to_resp->result = true;
    }
};


int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GoTo>());
  rclcpp::shutdown();
  return 0;
}
