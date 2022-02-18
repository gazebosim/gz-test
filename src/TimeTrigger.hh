#include <chrono>
#include <ignition/gazebo/World.hh>
#include "Trigger.hh"

namespace ignition
{
  namespace test
  {
  class TimeTrigger : public Trigger
  {
    public: TimeTrigger(const std::chrono::steady_clock::duration &_time);

    // Configure callback
    public: void Configure(const gazebo::Entity &_entity,
                           const std::shared_ptr<const sdf::Element> &_sdf,
                           gazebo::EntityComponentManager &_ecm,
                           gazebo::EventManager &_eventMgr) override;

    // Pre update update callback
    public: void PreUpdate(const gazebo::UpdateInfo &_info,
                gazebo::EntityComponentManager &_ecm) override;

    // Documentation inherited
    public: void Update(const gazebo::UpdateInfo &_info,
                  gazebo::EntityComponentManager &_ecm) override;

    // Post update callback
    public: void PostUpdate(const gazebo::UpdateInfo &_info,
                const gazebo::EntityComponentManager &_ecm) override;

    private: std::chrono::steady_clock::duration time;

    private: bool triggered{false};
    private: gazebo::World world;
  };
  }
}
