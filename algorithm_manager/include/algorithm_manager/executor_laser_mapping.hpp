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

#ifndef ALGORITHM_MANAGER__EXECUTOR_LASER_MAPPING_HPP_
#define ALGORITHM_MANAGER__EXECUTOR_LASER_MAPPING_HPP_

#include <string>
#include "algorithm_manager/executor_base.hpp"

namespace cyberdog
{
namespace algorithm
{

class ExecutorLaserMapping : public ExecutorBase
{
public:
  explicit ExecutorLaserMapping(std::string node_name);
  void Start(AlgorithmMGR::Goal::ConstSharedPtr goal) override;
  void Stop(
    const StopTaskSrv::Request::SharedPtr request,
    StopTaskSrv::Response::SharedPtr response) override;
  void Cancel() override;
  // void UpdateStatus(const ExecutorStatus & executor_status) override;
  // void GetFeedback(protocol::action::Navigation::Feedback::SharedPtr feedback) override;

private:
};  // class ExecutorLaserMapping
}  // namespace algorithm
}  // namespace cyberdog
#endif  // ALGORITHM_MANAGER__EXECUTOR_LASER_MAPPING_HPP_
