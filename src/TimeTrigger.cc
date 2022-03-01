#include <ignition/gazebo/Link.hh>
#include <ignition/gazebo/Model.hh>
#include <ignition/gazebo/components/Pose.hh>

#include "TimeTrigger.hh"

using namespace ignition;
using namespace test;

//////////////////////////////////////////////////
TimeTrigger::TimeTrigger(const std::chrono::steady_clock::duration &_time)
  : time(_time)
{
}

//////////////////////////////////////////////////
void TimeTrigger::Configure(const gazebo::Entity &_entity,
               const std::shared_ptr<const sdf::Element> &,
               gazebo::EntityComponentManager &,
               gazebo::EventManager &)
{
  this->world = gazebo::World(_entity);
}

//////////////////////////////////////////////////
void TimeTrigger::PreUpdate(const gazebo::UpdateInfo &,
    gazebo::EntityComponentManager &)
{
}

//////////////////////////////////////////////////
void TimeTrigger::Update(const gazebo::UpdateInfo &,
    gazebo::EntityComponentManager &)
{
}

//////////////////////////////////////////////////
void TimeTrigger::PostUpdate(const gazebo::UpdateInfo &_info,
    const gazebo::EntityComponentManager &_ecm)
{
  if (_info.simTime >= this->time && !this->triggered)
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
