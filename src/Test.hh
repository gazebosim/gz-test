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
#ifndef IGNITION_TEST_TEST_HH_
#define IGNITION_TEST_TEST_HH_

#include <yaml-cpp/yaml.h>

#include <ignition/gazebo/Server.hh>
#include <ignition/gazebo/ServerConfig.hh>
#include <ignition/gazebo/World.hh>

#include "msgs/results.pb.h"
#include "Trigger.hh"
#include "Util.hh"
#include "ignition/test/config.hh"

using namespace std::chrono_literals;

namespace ignition
{
  namespace test
  {
    // Inline bracket to help doxygen filtering.
    inline namespace IGNITION_TEST_VERSION_NAMESPACE {
    class Test :
            public gazebo::System,
            public gazebo::ISystemConfigure,
            public gazebo::ISystemPreUpdate,
            public gazebo::ISystemUpdate,
            public gazebo::ISystemPostUpdate

    {
      /// \brief Default constructor.
      public: Test();

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

      /// \brief Load a test.
      /// \param[in] _node The YAML node containing test information
      /// \return True if the test was loaded successfully.
      public: bool Load(const YAML::Node &_node);

      /// \brief Get the test's name.
      /// \return The name of the test.
      public: std::string Name() const;

      public: void FillResults(ignition::test::msgs::Test *_msg) const;

      /// \brief Check if a trigger with the given name exists.
      /// \return True if the trigger exists.
      public: bool HasTrigger(const std::string &_name) const;

      public: std::optional<bool> RunTriggerFunction(
                  const std::string &_triggerName,
                  const std::string &_functionName,
                  const std::string &_parameter);

      /// \brief Stop the test.
      public: void Stop();

      public: std::chrono::steady_clock::duration MaxDuration();

      /// \brief Get the time type (sim or real) associated with
      /// MaxDuration.
      /// \return The time type, sim or real, for max duration.
      public: TimeType MaxDurationType();

      public: void SetStopCallback(std::function<void()> &_cb);

      private: gazebo::World world;

      /// \brief Name of the test
      private: std::string name;

      /// \brief The list of triggers for the test.
      private: std::vector<std::unique_ptr<Trigger>> triggers;

      public: std::chrono::steady_clock::duration maxDuration{60s};
      public: TimeType maxDurationType{TimeType::SIM};

      public: std::function<void()> stopCb;
    };
    }
  }
}

#endif
