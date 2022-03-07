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
#ifndef IGNITION_TEST_TIMETRIGGER_HH_
#define IGNITION_TEST_TIMETRIGGER_HH_

#include <chrono>
#include <memory>
#include <ignition/gazebo/World.hh>

#include "ignition/test/config.hh"
#include "Trigger.hh"

namespace ignition
{
  namespace test
  {
    // Inline bracket to help doxygen filtering.
    inline namespace IGNITION_TEST_VERSION_NAMESPACE {
    class TimeTrigger :
      public Trigger,
      public std::enable_shared_from_this<TimeTrigger>
    {
      public: enum class Type
      {
        // Simulation time trigger type
        SIM,
        // Real time trigger type
        REAL
      };

      // Default constructor.
      public: TimeTrigger();

      public: virtual bool Load(const YAML::Node &_node) override;

      private: class TimeTriggerSystem :
            public gazebo::System,
            public gazebo::ISystemConfigure,
            public gazebo::ISystemPreUpdate,
            public gazebo::ISystemUpdate,
            public gazebo::ISystemPostUpdate

      {
        // Configure callback
        public: void Configure(const gazebo::Entity &_entity,
                               const std::shared_ptr<const sdf::Element> &_sdf,
                               gazebo::EntityComponentManager &_ecm,
                               gazebo::EventManager &_eventMgr) override;

        // Pre update update callback
        public: void PreUpdate(const gazebo::UpdateInfo &_info,
                    gazebo::EntityComponentManager &_ecm) override;

        // Documentation inherited
        public: void Update(const gazebo::UpdateInfo &_info,
                      gazebo::EntityComponentManager &_ecm) override;

        // Post update callback
        public: void PostUpdate(const gazebo::UpdateInfo &_info,
                    const gazebo::EntityComponentManager &_ecm) override;

        public: std::chrono::steady_clock::duration duration;

        public: bool triggered{false};

        public: gazebo::World world;

        public: TimeTrigger::Type type{TimeTrigger::Type::SIM};
      };
    };
    }
  }
}
#endif
