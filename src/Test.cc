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
#include <yaml-cpp/yaml.h>
#include "TimeTrigger.hh"
#include "Test.hh"

using namespace ignition;
using namespace test;

class Test::Implementation
{
  /// \brief Default constructor
  public: Implementation() = default;

  /// \brief Copy constructor
  public: Implementation(const Test::Implementation &_impl)
          : name(_impl.name), triggers(_impl.triggers)
  {}

  /// \brief The test name
  public: std::string name;

  /// \brief The list of triggers for the test.
  public: std::vector<Trigger> triggers;
};

/////////////////////////////////////////////////
Test::Test()
  : dataPtr(utils::MakeImpl<Implementation>())
{
}

/////////////////////////////////////////////////
bool Test::Load(const YAML::Node &_node)
{
  // The test name
  if (_node["name"])
    this->dataPtr->name = _node["name"].as<std::string>();

  // Load all the triggers
  for (YAML::const_iterator it = _node["triggers"].begin();
       it != _node["triggers"].end(); ++it)
  {
    std::string triggerType = (*it)["type"].as<std::string>();
    if (triggerType == "time")
    {
      TimeTrigger trigger;
      trigger.Load(*it);
      this->dataPtr->triggers.push_back(std::move(trigger));
    }
  }

  return true;
}

/////////////////////////////////////////////////
void Test::AddTriggersToServer(gazebo::Server &_server)
{
  for (Trigger &trigger : this->dataPtr->triggers)
  {
    igndbg << "Added system\n";
    _server.AddSystem(trigger.System());
  }
}
