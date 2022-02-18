#include <ignition/gazebo/Server.hh>
#include <ignition/gazebo/ServerConfig.hh>

namespace ignition
{
  namespace test
  {
  class Trigger :
      public gazebo::System,
      public gazebo::ISystemConfigure,
      public gazebo::ISystemPreUpdate,
      public gazebo::ISystemUpdate,
      public gazebo::ISystemPostUpdate
  {
    // Documentation inherited
    public: void Configure(
                  const gazebo::Entity &_entity,
                  const std::shared_ptr<const sdf::Element> &_sdf,
                  gazebo::EntityComponentManager &_ecm,
                  gazebo::EventManager &_eventMgr) override;

    // Documentation inherited
    public: void PreUpdate(const gazebo::UpdateInfo &_info,
                  gazebo::EntityComponentManager &_ecm) override;

    // Documentation inherited
    public: void Update(const gazebo::UpdateInfo &_info,
                  gazebo::EntityComponentManager &_ecm) override;

    // Documentation inherited
    public: void PostUpdate(const gazebo::UpdateInfo &_info,
                  const gazebo::EntityComponentManager &_ecm) override;
  };
  }
}
