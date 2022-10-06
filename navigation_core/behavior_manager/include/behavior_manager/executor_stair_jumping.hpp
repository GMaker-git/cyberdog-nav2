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

#ifndef BEHAVIOR_MANAGER__EXECUTOR_STAIR_JUMPING_HPP_
#define BEHAVIOR_MANAGER__EXECUTOR_STAIR_JUMPING_HPP_

#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int8.hpp"
#include "std_msgs/msg/bool.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "std_srvs/srv/set_bool.hpp"
#include "std_srvs/srv/trigger.hpp"
#include "protocol/srv/motion_result_cmd.hpp"
#include "cyberdog_debug/backtrace.hpp"
namespace cyberdog
{
namespace algorithm
{

class ExecutorStairJumping : public rclcpp::Node
{
public:
  enum class JumpingStatus : int8_t
  {
    kIdle,
    kAligning,
    kJumping,
    kJumped,
    kAbnorm,
  };
  ExecutorStairJumping();
  ~ExecutorStairJumping();
  void Execute(bool trigger)
  {
    auto request = std::make_shared<std_srvs::srv::Trigger::Request>();
    auto callback = [this](rclcpp::Client<std_srvs::srv::Trigger>::SharedFuture future){
      if(future.get()->success){
        jumping_status_ = JumpingStatus::kAligning;
      } else {
        jumping_status_ = JumpingStatus::kAbnorm;
      }
    };
    stair_align_trigger_client_->async_send_request(request, callback);
  }
  JumpingStatus GetStatus(){return jumping_status_;}
private:
  // void HandleStairDetectionCallback(const std_msgs::msg::Int8::SharedPtr msg)
  // {
  //   stair_detection_ = msg->data;
  // }
  void HandleStairAlginStatusCallback(const std_msgs::msg::Bool::SharedPtr msg)
  {
    stair_aligned_ = msg->data;
  }
  // void HandleTargetPoseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
  // {
  //   current_pose_ = msg;
  // }
  void Loop()
  {
    while(stair)
  }
  bool CheckTargetStatic()
  {

  }
  void DoAutonomouslyTracking();
  void DoStairJumping();
  // rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr stair_detected_sub_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr stair_align_finished_sub_;
  // rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr target_pose_sub_;
  rclcpp::Client<std_srvs::srv::SetBool>::SharedPtr stair_jump_client_;
  // rclcpp::Client<std_srvs::srv::SetBool>::SharedPtr tracking_switch_client_;
  // rclcpp::Client<std_srvs::srv::SetBool>::SharedPtr autonomously_tracking_client_;
  rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr stair_align_trigger_client_;
  rclcpp::Client<protocol::srv::MotionResultCmd>::SharedPtr motion_jump_client_;
  // geometry_msgs::msg::PoseStamped::SharedPtr current_pose_;
  Stage stage_;
  JumpingStatus jumping_status_;
  int8_t stair_detection_{0};
  bool stair_aligned_{false};

};  // class ModeDetector
}  // namespace algorithm
}  // namespace cyberdog
#endif  // BEHAVIOR_MANAGER__EXECUTOR_STAIR_JUMPING_HPP_
