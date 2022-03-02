#include <ignition/gazebo/Link.hh>
#include <ignition/gazebo/Model.hh>
#include <ignition/gazebo/components/Pose.hh>

#include <ignition/math/Helpers.hh>

#include "TimeTrigger.hh"

using namespace ignition;
using namespace test;

//////////////////////////////////////////////////
void TimeTrigger::TimeTriggerSystem::Configure(const gazebo::Entity &_entity,
               const std::shared_ptr<const sdf::Element> &,
               gazebo::EntityComponentManager &,
               gazebo::EventManager &)
{
  this->world = gazebo::World(_entity);
}

//////////////////////////////////////////////////
void TimeTrigger::TimeTriggerSystem::PreUpdate(const gazebo::UpdateInfo &,
    gazebo::EntityComponentManager &)
{
}

//////////////////////////////////////////////////
void TimeTrigger::TimeTriggerSystem::Update(const gazebo::UpdateInfo &,
    gazebo::EntityComponentManager &)
{
}

//////////////////////////////////////////////////
void TimeTrigger::TimeTriggerSystem::PostUpdate(const gazebo::UpdateInfo &_info,
    const gazebo::EntityComponentManager &_ecm)
{
  if (_info.simTime >= this->duration && !this->triggered)
  {
    std::cout << "Time trigger at time[" << _info.simTime.count() << "]\n";

    std::string name = "x1-b";
    if (this->world.ModelByName(_ecm, name))
    {
      gazebo::Model model(this->world.ModelByName(_ecm, name));
      auto pose = _ecm.ComponentData<gazebo::components::Pose>(model.Entity());
      if (pose)
      {
      std::cout << "Model Name[" << model.Name(_ecm) << "] pose[" << *pose << "]\n";
      }
    }

    this->triggered = true;
  }
}

//////////////////////////////////////////////////
bool TimeTrigger::Load(const YAML::Node &_node)
{
  auto system = std::make_shared<TimeTrigger::TimeTriggerSystem>();

  this->SetType(Trigger::TriggerType::TIME);

  if (_node["name"])
  {
    this->SetName(_node["name"].as<std::string>());
  }
  else
  {
    ignerr << "Time trigger is missing a name, skipping.\n";

    return false;
  }

  if (_node["time"])
  {
    YAML::Node timeNode = _node["time"];
    if (timeNode["duration"])
    {
      system->duration = math::stringToDuration(
          timeNode["duration"].as<std::string>());
    }
    else
    {
      ignerr << "Time trigger[" << this->Name()
        << "] is missing a duration, skipping.\n";
      return false;
    }

    if (timeNode["type"])
    {
      std::string timeType =
        common::lowercase(timeNode["type"].as<std::string>());
      if (timeType == "sim")
        system->type = TimeTrigger::Type::SIM;
      else if (timeType == "real")
        system->type = TimeTrigger::Type::REAL;
      else
      {
        ignerr << "Time trigger[" << this->Name()
          << "] is missing a time type, using simulation time.\n";
        return false;
      }
    }
  }
  else
  {
    ignerr << "Time trigger[" << this->Name()
      << "] is missing a time, skipping.\n";
    return false;
  }

  igndbg << "Created time trigger " << this->Name() << " with duration "
    << system->duration.count() << std::endl;

  this->SetSystem(system);
  return true;
}
