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
#include "RegionTrigger.hh"
#include "TimeTrigger.hh"
#include "Test.hh"

using namespace ignition;
using namespace test;

/////////////////////////////////////////////////
Test::Test()
{
}

/////////////////////////////////////////////////
bool Test::Load(const YAML::Node &_node)
{
  // The test name
  if (_node["name"])
    this->name = _node["name"].as<std::string>();

  // Load all the triggers
  for (YAML::const_iterator it = _node["triggers"].begin();
       it != _node["triggers"].end(); ++it)
  {
    std::string triggerType = (*it)["type"].as<std::string>();
    if (triggerType == "time")
    {
      auto trigger = std::make_unique<TimeTrigger>();
      trigger->Load(*it);
      this->triggers.push_back(std::move(trigger));
    }
    else if (triggerType == "region")
    {
      auto trigger = std::make_unique<RegionTrigger>();
      trigger->Load(*it);
      this->triggers.push_back(std::move(trigger));
    }
  }

  return true;
}

/////////////////////////////////////////////////
std::string Test::Name() const
{
  return this->name;
}

//////////////////////////////////////////////////
void Test::Configure(
    const gazebo::Entity &_entity,
    const std::shared_ptr<const sdf::Element> &,
    gazebo::EntityComponentManager &,
    gazebo::EventManager &)
{
  this->world = gazebo::World(_entity);
}

//////////////////////////////////////////////////
void Test::PreUpdate(const gazebo::UpdateInfo &,
    gazebo::EntityComponentManager &)
{
}

//////////////////////////////////////////////////
void Test::Update(const gazebo::UpdateInfo &,
    gazebo::EntityComponentManager &)
{
}

//////////////////////////////////////////////////
void Test::PostUpdate(
    const gazebo::UpdateInfo &_info,
    const gazebo::EntityComponentManager &_ecm)
{
  for (std::unique_ptr<Trigger> &trigger : this->triggers)
  {
    trigger->Update(_info, this->world, _ecm);
  }
}

//////////////////////////////////////////////////
void Test::FillResults(ignition::test::msgs::Test *_msg) const
{
  _msg->set_name(this->Name());

  bool allPassed = true;
  for (const std::unique_ptr<Trigger> &trigger : this->triggers)
  {
    test::msgs::Trigger *triggerMsg = _msg->add_triggers();
    triggerMsg->set_name(trigger->Name());
    std::optional<bool> triggerResult = trigger->Result();

    if (triggerResult)
    {
      triggerMsg->set_pass(*triggerResult);
      allPassed = allPassed && *triggerResult;
    }
    else
    {
      allPassed = false;
    }
  }

  _msg->set_pass(allPassed);
}
