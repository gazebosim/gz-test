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
#include <ignition/gazebo/Util.hh>
#include <ignition/gazebo/components/Model.hh>
#include <ignition/gazebo/components/Name.hh>
#include <ignition/gazebo/components/Pose.hh>

#include <ignition/math/Helpers.hh>

#include "Util.hh"
#include "RegionTrigger.hh"

using namespace ignition;
using namespace test;


//////////////////////////////////////////////////
void RegionTrigger::Update(const gazebo::UpdateInfo &_info,
    Test *_test, const gazebo::EntityComponentManager &_ecm)
{
  // Update what this region contains.
  _ecm.Each<gazebo::components::Model, gazebo::components::Name>(
        [&](const gazebo::Entity &_entity,
            const gazebo::components::Model *,
            const gazebo::components::Name *_name) -> bool
        {
          math::Pose3d pose = gazebo::worldPose(_entity, _ecm);
          std::string modelName = _name->Data();

          if (this->box.Contains(pose.Pos()))
          {
            if (!this->Contains(modelName))
            {
              this->containedEntities.insert(modelName);
              this->SetResult(this->RunOnCommands(_info, _test, _ecm));
            }
          }
          else if (this->containedEntities.find(modelName) !=
              this->containedEntities.end())
          {
            this->containedEntities.erase(modelName);
          }
          return true;
        });
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

  if (_node["geometry"])
  {
    YAML::Node geomNode = _node["geometry"];
    math::Vector3d pos = math::Vector3d::Zero;
    math::Vector3d size = math::Vector3d::One;

    if (geomNode["pos"])
      pos = yamlParseVector3d(geomNode["pos"]);

    if (geomNode["box"] && geomNode["box"]["size"])
      size = yamlParseVector3d(geomNode["box"]["size"]);
    math::Vector3d halfSize = size / 2.0;

    this->box = math::AxisAlignedBox(pos - halfSize, pos + halfSize);
  }
  else
  {
    ignerr << "Region trigger[" << this->Name()
      << "] is missing a geometry, skipping.\n";
    return false;
  }

  if (_node["on"])
    this->LoadOnCommands(_node["on"]);

  igndbg << "Created region trigger " << this->Name() << " with box ["
    << this->box << "].\n";

  std::function<bool(const std::string &)> func =
    std::bind(&RegionTrigger::Contains, this, std::placeholders::_1);
  this->RegisterFunction("contains", func);

  return true;
}

//////////////////////////////////////////////////
bool RegionTrigger::Contains(const std::string &_name)
{
  return this->containedEntities.find(_name) != this->containedEntities.end();
}
