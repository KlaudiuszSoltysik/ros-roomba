#include <chrono>
#include <functional>
#include <memory>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs/legacy/constants_c.h>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "std_msgs/msg/u_int8_multi_array.hpp"


class CameraTranslate : public rclcpp::Node {
  public:
    CameraTranslate() : Node("camera_translate") {
      camera_sub = this->create_subscription<sensor_msgs::msg::Image>("camera", 10, std::bind(&CameraTranslate::camera_sub_cb, this, std::placeholders::_1));
      camera_translated_pub = this->create_publisher<std_msgs::msg::UInt8MultiArray>("camera_translated", 10);

      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The camera translate node ran successfully");
    }

  private:
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr camera_sub;
    rclcpp::Publisher<std_msgs::msg::UInt8MultiArray>::SharedPtr camera_translated_pub;

    void camera_sub_cb(const sensor_msgs::msg::Image & msg) const {
      cv::Mat image(msg.height, msg.width, CV_8UC3);
      for (unsigned int i = 0; i < msg.height; ++i) {
        for (unsigned int j = 0; j < msg.width; ++j) {
          image.at<cv::Vec3b>(i, j)[0] = msg.data[i * msg.width * 3 + j * 3 + 0];
          image.at<cv::Vec3b>(i, j)[1] = msg.data[i * msg.width * 3 + j * 3 + 1];
          image.at<cv::Vec3b>(i, j)[2] = msg.data[i * msg.width * 3 + j * 3 + 2];
        }
      }

      std::vector<uint8_t> jpeg_data;
      cv::imencode(".jpeg", image, jpeg_data);

      std_msgs::msg::UInt8MultiArray translated_msg;
      translated_msg.data = jpeg_data;
      camera_translated_pub->publish(translated_msg);
    }
};


int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CameraTranslate>());
  rclcpp::shutdown();
  return 0;
}
