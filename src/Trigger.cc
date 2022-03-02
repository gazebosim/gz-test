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
#include "Trigger.hh"

using namespace ignition;
using namespace test;

class Trigger::Implementation
{
  public: std::string name{""};

  public: TriggerType type{Trigger::TriggerType::UNDEFINED};

  public: std::shared_ptr<gazebo::System> system;
};

/////////////////////////////////////////////////
Trigger::Trigger()
  : dataPtr(utils::MakeImpl<Implementation>())
{
}

//////////////////////////////////////////////////
bool Trigger::Load(const YAML::Node &)
{
  return false;
}

//////////////////////////////////////////////////
std::shared_ptr<gazebo::System> Trigger::System() const
{
  return this->dataPtr->system;
}

//////////////////////////////////////////////////
void Trigger::SetSystem(std::shared_ptr<gazebo::System> _system)
{
  this->dataPtr->system = _system;
}

//////////////////////////////////////////////////
std::string Trigger::Name() const
{
  return this->dataPtr->name;
}

//////////////////////////////////////////////////
void Trigger::SetName(const std::string &_name)
{
  this->dataPtr->name = _name;
}

//////////////////////////////////////////////////
Trigger::TriggerType Trigger::Type() const
{
  return this->dataPtr->type;
}

//////////////////////////////////////////////////
void Trigger::SetType(const TriggerType &_type)
{
  this->dataPtr->type = _type;
}
