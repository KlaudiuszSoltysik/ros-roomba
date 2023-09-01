#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "sensor_msgs/msg/imu.hpp"


class Map : public rclcpp::Node {
  public:
    Map() : Node("map") {
      // Prompt user about succesfully created node
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The map node ran successfully");
    }

  private:
    
};


int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Map>());
  rclcpp::shutdown();
  return 0;
}
