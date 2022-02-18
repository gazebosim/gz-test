#include <ignition/gazebo/Link.hh>
#include <ignition/gazebo/Model.hh>

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
    gazebo::Model model(this->world.ModelByName(_ecm, "sphere"));
    std::cout << "pose[" << *(gazebo::Link(model.Links(_ecm)[0]).WorldPose(_ecm)) << "]\n";

    this->triggered = true;
  }
}
