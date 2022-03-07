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

/////////////////////////////////////////////////
Trigger::Trigger()
{
}

//////////////////////////////////////////////////
bool Trigger::Load(const YAML::Node &)
{
  return false;
}

//////////////////////////////////////////////////
bool Trigger::LoadOnCommands(const YAML::Node &_node)
{
  if (!_node.IsSequence())
    return false;

  // Iterate over the sequence of commands.
  for (YAML::const_iterator it = _node.begin(); it!=_node.end(); ++it)
  {
    if ((*it)["run"])
    {
      std::string cmd = (*it)["run"].as<std::string>();
      std::cout << "LoadedCommand[" << cmd << "]\n";
      this->commands.push_back(cmd);
    }
  }
  return true;
}

//////////////////////////////////////////////////
bool Trigger::RunOnCommands()
{
  std::cout << "Commands for[" << this->name << "] size[" << this->commands.size() << "]\n";
  /*for (const std::string &cmd : this->commands)
  {
    std::cout << "Running command[" << cmd << "]\n";
  }*/

  return true;
}

//////////////////////////////////////////////////
std::shared_ptr<gazebo::System> Trigger::System() const
{
  return this->system;
}

//////////////////////////////////////////////////
void Trigger::SetSystem(std::shared_ptr<gazebo::System> _system)
{
  this->system = _system;
}

//////////////////////////////////////////////////
std::string Trigger::Name() const
{
  return this->name;
}

//////////////////////////////////////////////////
void Trigger::SetName(const std::string &_name)
{
  this->name = _name;
}

//////////////////////////////////////////////////
Trigger::TriggerType Trigger::Type() const
{
  return this->type;
}

//////////////////////////////////////////////////
void Trigger::SetType(const TriggerType &_type)
{
  this->type = _type;
}
