#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"


class MapRoom : public rclcpp::Node {
  public:
    MapRoom() : Node("map_room") {
      // Prompt user about succesfully created node
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "The map room node ran successfully");
    }
};


int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MapRoom>());
  rclcpp::shutdown();
  return 0;
}
