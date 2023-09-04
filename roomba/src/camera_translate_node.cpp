#include <chrono>
#include <functional>
#include <memory>
#include <vector>

#include <opencv2/opencv.hpp>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "std_msgs/msg/string.hpp"


class CameraTranslate : public rclcpp::Node {
  public:
    CameraTranslate() : Node("camera_translate") {
      camera_sub = this->create_subscription<sensor_msgs::msg::Image>("camera", 10, std::bind(&CameraTranslate::camera_sub_cb, this, std::placeholders::_1));
      camera_translated_pub = this->create_publisher<std_msgs::msg::String>("camera_translated", 10);

      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The camera translate node ran successfully");
    }

  private:
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr camera_sub;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr camera_translated_pub;

    void camera_sub_cb(const sensor_msgs::msg::Image & msg) const {
      cv::Mat image(msg.height, msg.width, CV_8UC3, const_cast<uint8_t*>(msg.data.data()));

      std::vector<uint8_t> jpeg_data;
      cv::imencode(".jpeg", image, jpeg_data);

      std::string jpeg_string(jpeg_data.begin(), jpeg_data.end());
      
      std_msgs::msg::String translated_msg;
      translated_msg.data = jpeg_string;
      camera_translated_pub->publish(translated_msg);
    }
};


int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CameraTranslate>());
  rclcpp::shutdown();
  return 0;
}
