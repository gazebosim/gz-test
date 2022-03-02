#ifndef IGNITION_TEST_TIMETRIGGER_HH_
#define IGNITION_TEST_TIMETRIGGER_HH_

#include <chrono>
#include <ignition/gazebo/World.hh>
#include "Trigger.hh"

namespace ignition
{
  namespace test
  {
    class TimeTrigger : public Trigger
    {
      public: enum class Type
      {
        // Simulation time trigger type
        SIM,
        // Real time trigger type
        REAL
      };

      // Default constructor.
      public: TimeTrigger() = default;

      public: virtual bool Load(const YAML::Node &_node) override;

      private: class TimeTriggerSystem :
            public gazebo::System,
            public gazebo::ISystemConfigure,
            public gazebo::ISystemPreUpdate,
            public gazebo::ISystemUpdate,
            public gazebo::ISystemPostUpdate

      {
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

        public: std::chrono::steady_clock::duration duration;

        public: bool triggered{false};

        public: gazebo::World world;

        public: TimeTrigger::Type type{TimeTrigger::Type::SIM};
      };
    };
  }
}
#endif
