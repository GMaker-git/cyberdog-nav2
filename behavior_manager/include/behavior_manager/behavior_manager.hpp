// Copyright (c) 2021 Beijing Xiaomi Mobile Software Co., Ltd. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef BEHAVIOR_MANAGER__BEHAVIOR_MANAGER_HPP_
#define BEHAVIOR_MANAGER__BEHAVIOR_MANAGER_HPP_

#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int8.hpp"
#include "std_msgs/msg/bool.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "std_srvs/srv/set_bool.hpp"
#include "std_srvs/srv/trigger.hpp"
#include "protocol/srv/motion_result_cmd.hpp"
#include "cyberdog_debug/backtrace.hpp"
#include "behavior_manager/mode_detector.hpp"
#include "behavior_manager/executor_auto_tracking.hpp"
#include "behavior_manager/executor_stair_jumping.hpp"
namespace cyberdog
{
namespace algorithm
{

class BehaviorManager
{
public:
  enum class Status : uint8_t
  {
    kNormTracking,
    kAutoTracking,
    kStairJumping,
    kAbnorm
  };
  explicit BehaviorManager(const std::string & node_name)
  {
    node_ = std::make_shared<rclcpp::Node>(node_name);
    mode_detector_ = std::make_shared<ModeDetector>("mode_detector");
    mode_detector_->Init(
      std::bind(&BehaviorManager::DoStairJump, this, std::placeholders::_1),
      std::bind(&BehaviorManager::DoAutoTracking, this),
      std::bind(&BehaviorManager::DoNormallyTracking, this, std::placeholders::_1)
    );
    executor_auto_tracking_ = std::make_shared<ExecutorAutoTracking>("executor_auto_tracking");
    executor_stair_jumping_ = std::make_shared<ExecutorStairJumping>("executor_stair_jumping");
    executor_stair_jumping_->Init(
      std::bind(&BehaviorManager::HandleJumped, this),
      std::bind(&BehaviorManager::HandleJumpFailed, this));
    tracking_switch_client_ = node_->create_client<std_srvs::srv::SetBool>("tracking_command");

    std::thread{[this](){rclcpp::spin(node_);}}.detach();
    // ros_executor_ = std::make_shared<rclcpp::executors::MultiThreadedExecutor>();
    // ros_executor_->add_node(mode_detector_);
    // ros_executor_->add_node(executor_auto_tracking_);
    // ros_executor_->add_node(executor_stair_jumping_);
    // std::thread{[this](){ros_executor_->spin();}}.detach();
  }
  ~BehaviorManager(){}
private:
  bool CheckStatusValid()
  {
    return status_ == Status::kNormTracking;
  }
  void DoAutoTracking()
  {
    if(!CheckStatusValid()) {
      return;
    }
    DoNormallyTracking(false);
    executor_auto_tracking_->Execute(true);
  }
  void DoStairJump(bool trigger)
  {
    INFO("111");
    if(!CheckStatusValid()) {
      return;
    }
    DoNormallyTracking(false);
    executor_stair_jumping_->Execute(trigger);
    status_ = Status::kStairJumping;
    INFO("999");
  }
  bool DoNormallyTracking(bool trigger)
  {
    auto request = std::make_shared<std_srvs::srv::SetBool::Request>();
    request->data = trigger;
    auto future = tracking_switch_client_->async_send_request(request);
    if (future.wait_for(std::chrono::milliseconds(2000)) == std::future_status::ready) {
      return future.get()->success;
    }
    return false;
  }
  bool HandleJumped()
  {
    DoNormallyTracking(true);
    status_ = Status::kNormTracking;
    return true;
  }
  bool HandleJumpFailed()
  {
    status_ = Status::kAbnorm;
    HandleAbnormStatus();
    return true;
  }
  void HandleAbnormStatus()
  {}
  // void GetMode(const ModeDetector::Stage & stage){ stage_detected_ = stage; }
  rclcpp::Node::SharedPtr node_;
  rclcpp::Client<std_srvs::srv::SetBool>::SharedPtr tracking_switch_client_;
  ModeDetector::Stage stage_working_, stage_detected_;
  std::shared_ptr<ModeDetector> mode_detector_;
  std::shared_ptr<ExecutorAutoTracking> executor_auto_tracking_;
  std::shared_ptr<ExecutorStairJumping> executor_stair_jumping_;
  rclcpp::Executor::SharedPtr ros_executor_;
  Status status_{Status::kNormTracking};
  bool stair_detected_{false}, stair_aligned_{false}, stair_align_timeout_{false};
  bool stair_possible_jump_{false};

};  // class BehaviorManager
}  // namespace algorithm
}  // namespace cyberdog
#endif  // BEHAVIOR_MANAGER__BEHAVIOR_MANAGER_HPP_
