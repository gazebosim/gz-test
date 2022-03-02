#ifndef IGNITION_TEST_TRIGGER_HH_
#define IGNITION_TEST_TRIGGER_HH_

#include <yaml-cpp/yaml.h>
#include <ignition/gazebo/Server.hh>
#include <ignition/gazebo/ServerConfig.hh>

namespace ignition
{
  namespace test
  {
    class Trigger
    {
      public: enum class TriggerType
      {
        // A time trigger
        TIME,
        // A region trigger
        REGION,

        UNDEFINED,
      };

      public: virtual bool Load(const YAML::Node &_node);

      public: std::shared_ptr<gazebo::System> System() const;
      public: void SetSystem(std::shared_ptr<gazebo::System> _system);

      public: std::string Name() const;

      public: void SetName(const std::string &_name);

      public: TriggerType Type() const;

      public: void SetType(const TriggerType &_type);

      private: std::string name;

      private: TriggerType type;

      private: std::shared_ptr<gazebo::System> system;
    };
  }
}
#endif
