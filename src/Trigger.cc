#include "Trigger.hh"

using namespace ignition;
using namespace test;

//////////////////////////////////////////////////
bool Trigger::Load(const YAML::Node &)
{
  return false;
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
