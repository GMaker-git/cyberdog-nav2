//
// Copyright (c) 2021 Beijing Xiaomi Mobile Software Co., Ltd. All rights reserved.
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

#include "cyberdog_maps_manager/maps_protocol.hpp"

namespace cyberdog
{
namespace maps_manager
{

MapsProtocol::MapsProtocol()
{
}

MapsProtocol::~MapsProtocol()
{
}

bool MapsProtocol::Save(const Request & request)
{
  return true;
}

bool MapsProtocol::Delete(const Request & request)
{
  return true;
}

bool MapsProtocol::Update(const Request & request)
{
  return true;
}

bool MapsProtocol::Query(const Request & request)
{
  return true;
}

}  // namespace maps_manager
}  // namespace cyberdog
