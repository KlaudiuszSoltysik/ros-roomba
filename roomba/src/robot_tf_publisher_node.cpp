#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"

class TfPublisher : public rclcpp::Node {
  public:
    TfPublisher() : Node("tf_publisher") {
      // Create odometry topic subscriber
      odometry_sub = create_subscription<nav_msgs::msg::Odometry>("odometry", 10, std::bind(&TfPublisher::odometrySubCb, this, std::placeholders::_1));

      // Initialize the transform broadcaster
      tf_broadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

      // Prompt user about succesfully created node
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The robot tf publisher node ran successfully");
    }

  private:
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odometry_sub;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster;

    void odometrySubCb(const nav_msgs::msg::Odometry & msg) {
      geometry_msgs::msg::TransformStamped t;

      t.header.stamp = this->get_clock()->now();
      t.header.frame_id = "world";
      t.child_frame_id = "roomba";

      t.transform.translation.x = msg.pose.pose.position.x;
      t.transform.translation.y = msg.pose.pose.position.y;
      t.transform.translation.z = 0.0;
      t.transform.rotation.x = msg.pose.pose.orientation.x;
      t.transform.rotation.y = msg.pose.pose.orientation.y;
      t.transform.rotation.z = msg.pose.pose.orientation.z;
      t.transform.rotation.w = msg.pose.pose.orientation.w;

      tf_broadcaster->sendTransform(t);
    }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TfPublisher>());
  rclcpp::shutdown();
  return 0;
}
