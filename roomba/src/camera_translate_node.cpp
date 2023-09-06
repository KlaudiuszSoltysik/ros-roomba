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

    const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

    void camera_sub_cb(const sensor_msgs::msg::Image & msg) {
      cv::Mat bgr_img(msg.height, msg.width, CV_8UC3, const_cast<uint8_t*>(msg.data.data()));

      cv::Mat rgb_img;
      cv::cvtColor(bgr_img, rgb_img, cv::COLOR_BGR2RGB);

      std::vector<uint8_t> jpeg_data;
      cv::imencode(".jpeg", rgb_img, jpeg_data);
      auto *enc_msg = reinterpret_cast<unsigned char*>(jpeg_data.data());
      std::string encoded = base64_encode(enc_msg, jpeg_data.size());

      std_msgs::msg::String translated_msg;
      translated_msg.data = encoded;
      camera_translated_pub->publish(translated_msg);
    }

    std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
      std::string ret;
      int i = 0;
      int j = 0;
      unsigned char char_array_3[3];
      unsigned char char_array_4[4];

      while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
          char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
          char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
          char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
          char_array_4[3] = char_array_3[2] & 0x3f;

          for(i = 0; (i <4) ; i++)
            ret += base64_chars[char_array_4[i]];
          i = 0;
        }
      }

      if (i)
      {
        for(j = i; j < 3; j++)
          char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
          ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
          ret += '=';

      }

      return ret;

    }
};


int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CameraTranslate>());
  rclcpp::shutdown();
  return 0;
}
