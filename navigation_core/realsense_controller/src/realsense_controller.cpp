#include "realsense_controller/realsense_controller.hpp"

#include <cstdio>
int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CYBERDOG_NAV::RealsenseController>();
  rclcpp::executors::MultiThreadedExecutor exec_;
  exec_.add_node(node->get_node_base_interface());
  exec_.spin();
  rclcpp::shutdown();
  return 0;
}
