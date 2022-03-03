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
#ifndef IGNITION_TEST_UTILS_HH_
#define IGNITION_TEST_UTILS_HH_

#include <yaml-cpp/yaml.h>
#include <ignition/math/Vector3.hh>
#include <ignition/math/Pose3.hh>

#include "ignition/test/config.hh"

namespace ignition
{
  namespace test
  {
    // Inline bracket to help doxygen filtering.
    inline namespace IGNITION_TEST_VERSION_NAMESPACE {
      math::Vector3d yamlParseVector3d(const YAML::Node &_node);
      math::Pose3d yamlParsePose3d(const YAML::Node &_node);
    }
  }
}

#endif
