/*
 * Copyright (C) 2022 Open Source Robotics Foundation
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
#ifndef IGNITION_TEST_REGIONTRIGGER_HH_
#define IGNITION_TEST_REGIONTRIGGER_HH_

#include <chrono>
#include <ignition/gazebo/World.hh>

#include "ignition/test/config.hh"
#include "Trigger.hh"

namespace ignition
{
  namespace test
  {
  // Inline bracket to help doxygen filtering.
  inline namespace IGNITION_TEST_VERSION_NAMESPACE {

    class RegionTrigger : public Trigger
    {
      // Default constructor.
      public: RegionTrigger() = default;

      public: virtual bool Load(const YAML::Node &_node) override;

      // Documentation inherited
      public: void Update(const gazebo::UpdateInfo &_info,
                  Test *_test,
                  const gazebo::EntityComponentManager &_ecm) override;

      public: bool Contains(const std::string &_name);

      protected: void ResetImpl() override final;

      public: math::AxisAlignedBox box;
      public: std::unordered_set<std::string> containedEntities;
    };
    }
  }
}
#endif
