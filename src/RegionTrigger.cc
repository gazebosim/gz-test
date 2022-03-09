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
#include <ignition/gazebo/Link.hh>
#include <ignition/gazebo/Model.hh>
#include <ignition/gazebo/components/Pose.hh>

#include <ignition/math/Helpers.hh>

#include "Util.hh"
#include "RegionTrigger.hh"

using namespace ignition;
using namespace test;


//////////////////////////////////////////////////
void RegionTrigger::Update(const gazebo::UpdateInfo &,
    const gazebo::World &, const gazebo::EntityComponentManager &)
{
}

//////////////////////////////////////////////////
bool RegionTrigger::Load(const YAML::Node &_node)
{
  this->SetType(Trigger::TriggerType::REGION);

  if (_node["name"])
  {
    this->SetName(_node["name"].as<std::string>());
  }
  else
  {
    ignerr << "Region trigger is missing a name, skipping.\n";
    return false;
  }

  if (_node["box"])
  {
    YAML::Node boxNode = _node["box"];
    YAML::Node boxPosNode = boxNode["pos"];
    YAML::Node boxSizeNode = boxNode["size"];

    math::Vector3d pos = yamlParseVector3d(boxNode["pos"]);
    math::Vector3d size = yamlParseVector3d(boxNode["size"]);
    math::Vector3d halfSize = size / 2.0;

    this->box = math::AxisAlignedBox(pos - halfSize, pos + halfSize);
  }
  else
  {
    ignerr << "Region trigger[" << this->Name()
      << "] is missing a box, skipping.\n";
    return false;
  }

  if (_node["on"])
    this->LoadOnCommands(_node["on"]);

  igndbg << "Created region trigger " << this->Name() << " with box ["
    << this->box << "].\n";

  return true;
}
