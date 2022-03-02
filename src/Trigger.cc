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
