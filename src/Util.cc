/*
 * Copyright 2022 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <ignition/math/Quaternion.hh>
#include "Util.hh"

namespace ignition
{
namespace test
{
// Inline bracket to help doxygen filtering.
inline namespace IGNITION_TEST_VERSION_NAMESPACE {

//////////////////////////////////////////////////
math::Vector3d yamlParseVector3d(const YAML::Node &_node)
{
  math::Vector3d result;
  if (_node["x"])
    result.X(_node["x"].as<double>());
  if (_node["y"])
    result.Y(_node["y"].as<double>());
  if (_node["z"])
    result.Z(_node["z"].as<double>());

  return result;
}

//////////////////////////////////////////////////
math::Pose3d yamlParsePose3d(const YAML::Node &_node)
{
  math::Vector3d rpy;

  if (_node["roll"])
    rpy.X(_node["roll"].as<double>());
  if (_node["pitch"])
    rpy.Y(_node["pitch"].as<double>());
  if (_node["yaw"])
    rpy.Z(_node["yaw"].as<double>());

  return math::Pose3d(yamlParseVector3d(_node), math::Quaterniond(rpy));
}
}
}
}
