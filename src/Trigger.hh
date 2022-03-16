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
#ifndef IGNITION_TEST_TRIGGER_HH_
#define IGNITION_TEST_TRIGGER_HH_

#include <yaml-cpp/yaml.h>
#include <map>
#include <string>
#include <vector>

#include <ignition/gazebo/Server.hh>
#include <ignition/gazebo/ServerConfig.hh>
#include <ignition/gazebo/World.hh>

#include "ignition/test/config.hh"

namespace ignition
{
  namespace test
  {
    // Inline bracket to help doxygen filtering.
    inline namespace IGNITION_TEST_VERSION_NAMESPACE {
    class Test;

    /// \brief Base class for all test triggers.
    class Trigger
    {
      /// \brief The available trigger types.
      public: enum class TriggerType
      {
        /// A time trigger
        TIME,

        /// A region trigger
        REGION,

        /// Undefine trigger type.
        UNDEFINED,
      };

      /// \brief Default constructor
      public: Trigger();

      /// \brief Load the trigger.
      /// \param[in] _node The YAML node to load.
      public: virtual bool Load(const YAML::Node &_node);

      public: virtual void Update(const gazebo::UpdateInfo &_info, Test *_test,
                  const gazebo::EntityComponentManager &_ecm) = 0;

      /// \brief Load all of the "on:" commands.
      /// \param[in] _node The YAML node that has the "on:" tag.
      /// \return True on success.
      public: bool LoadOnCommands(const YAML::Node &_node);

      /// \brief Check the expectationsj
      /// \return True on success.
      public: bool CheckExpectations(const gazebo::UpdateInfo &_info,
                  Test *_test, const gazebo::EntityComponentManager &_ecm);

      /// \brief Run the loaded "on:" commands.
      /// \return True on success.
      public: bool RunOnCommands(const gazebo::UpdateInfo &_info,
                  Test *_test, const gazebo::EntityComponentManager &_ecm);

      public: bool RunExecutable(const std::vector<std::string> &_cmd);

      /// \brief Get the trigger name.
      /// \return The trigger's name.
      public: std::string Name() const;

      /// \brief Set the trigger name.
      /// \param[in] _name The trigger name.
      public: void SetName(const std::string &_name);

      /// \brief Get the trigger type.
      /// \return The trigger type
      public: TriggerType Type() const;

      /// \brief Set the trigger type
      /// \param[in] _type The trigger type.
      public: void SetType(const TriggerType &_type);

      public: void SetResult(bool _passed);
      public: std::optional<bool> Result() const;

      public: std::optional<bool> ParseFunction(Test *_test,
                  const std::string &_str);

      public: std::optional<bool> RunFunction(const std::string &_name,
                  const std::string &_param);

      protected: void RegisterFunction(const std::string &_name,
                     std::function<bool(const std::string &)> &_func);

      private: std::optional<bool> ParseEquation(
                   const gazebo::UpdateInfo &_info,
                   const std::string &_str,
                   const gazebo::EntityComponentManager &_ecm);
      private: std::optional<double> ParseValue(const std::string &_str,
                   const gazebo::UpdateInfo &_info,
                   const gazebo::EntityComponentManager &_ecm);

      private: std::optional<double> ParsePoseProperty(
                   const math::Pose3d &_pose,
                   const std::string &_propertyStr);

      private: std::string name{""};

      private: TriggerType type{Trigger::TriggerType::UNDEFINED};

      private: std::vector<std::string> commands;
      private: std::vector<std::string> expectations;
      private: std::map<std::string, std::function<bool(const std::string &)>>
               functions;

      /// \brief Optional result, where std::nullopt means that there is no
      /// result.
      private: std::optional<bool> result;
    };
    }
  }
}
#endif
