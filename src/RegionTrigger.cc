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
void RegionTrigger::RegionTriggerSystem::Configure(
    const gazebo::Entity &_entity,
    const std::shared_ptr<const sdf::Element> &,
    gazebo::EntityComponentManager &,
    gazebo::EventManager &)
{
  this->world = gazebo::World(_entity);
}

//////////////////////////////////////////////////
void RegionTrigger::RegionTriggerSystem::PreUpdate(const gazebo::UpdateInfo &,
    gazebo::EntityComponentManager &)
{
}

//////////////////////////////////////////////////
void RegionTrigger::RegionTriggerSystem::Update(const gazebo::UpdateInfo &,
    gazebo::EntityComponentManager &)
{
}

//////////////////////////////////////////////////
void RegionTrigger::RegionTriggerSystem::PostUpdate(
    const gazebo::UpdateInfo & /*_info*/,
    const gazebo::EntityComponentManager &/*_ecm*/)
{
  /*if (_info.simRegion >= this->duration && !this->triggered)
  {
    igndbg << "Region trigger at time[" << _info.simRegion.count() << "]\n";

    //std::string name = "x1-b";
    //if (this->world.ModelByName(_ecm, name))
    //{
    //  gazebo::Model model(this->world.ModelByName(_ecm, name));
    //  auto pose = _ecm.ComponentData<gazebo::components::Pose>(model.Entity());
    //  if (pose)
    //  {
    //  std::cout << "Model Name[" << model.Name(_ecm) << "] pose[" << *pose << "]\n";
    //  }
    //}

    this->triggered = true;
  }
  */
}

//////////////////////////////////////////////////
bool RegionTrigger::Load(const YAML::Node &_node)
{
  auto system = std::make_shared<RegionTrigger::RegionTriggerSystem>();

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

    system->box = math::AxisAlignedBox(pos - halfSize, pos + halfSize);
  }
  else
  {
    ignerr << "Region trigger[" << this->Name()
      << "] is missing a box, skipping.\n";
    return false;
  }

  igndbg << "Created region trigger " << this->Name() << " with box ["
    << system->box << "].\n";

  this->SetSystem(system);
  return true;
}
