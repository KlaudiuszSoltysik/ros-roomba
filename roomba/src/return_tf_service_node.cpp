#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "custom_interfaces/srv/return_tf.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"


class ReturnTf : public rclcpp::Node {
  public:
    ReturnTf() : Node("return_tf") {
      // Create return tf service server 
      srv = create_service<custom_interfaces::srv::ReturnTf>("return_tf", std::bind(&ReturnTf::ReturnTfCb, this, std::placeholders::_1, std::placeholders::_2));

      // Create tf buffer and listener
      tf_buffer = std::make_unique<tf2_ros::Buffer>(get_clock());
      tf_listener = std::make_shared<tf2_ros::TransformListener>(*tf_buffer);

      // Prompt user about succesfully created node
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The return tf service node ran successfully");
    }

  private:
    rclcpp::Service<custom_interfaces::srv::ReturnTf>::SharedPtr srv;
    std::unique_ptr<tf2_ros::Buffer> tf_buffer;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener;
    geometry_msgs::msg::TransformStamped tf;

    void ReturnTfCb(const std::shared_ptr<custom_interfaces::srv::ReturnTf::Request> req, std::shared_ptr<custom_interfaces::srv::ReturnTf::Response> resp) {
      tf = tf_buffer->lookupTransform("world", "roomba", tf2::TimePointZero);

      tf.transform.translation.x += req->x;
      tf.transform.translation.y += req->y;
      
      resp->tf = tf;
    }
};


int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ReturnTf>());
  rclcpp::shutdown();
  return 0;
}
