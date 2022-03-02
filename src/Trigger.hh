#ifndef IGNITION_TEST_TRIGGER_HH_
#define IGNITION_TEST_TRIGGER_HH_

#include <yaml-cpp/yaml.h>
#include <ignition/gazebo/Server.hh>
#include <ignition/gazebo/ServerConfig.hh>

namespace ignition
{
  namespace test
  {
    /// \brief Base class for all test triggers.
    class Trigger
    {
      /// \brief The available trigger types.
      public: enum class TriggerType
      {
        /// A time trigger
        TIME,

        /// A region trigger
        REGION,

        /// Undefine trigger type.
        UNDEFINED,
      };

      /// \brief Default constructor
      public: Trigger();

      /// \brief Load the trigger.
      /// \param[in] _node The YAML node to load.
      public: virtual bool Load(const YAML::Node &_node);

      /// \brief Get the trigger's Gazebo system.
      /// \return The trigger's system.
      public: std::shared_ptr<gazebo::System> System() const;

      /// \brief Set the system.
      /// \param[in] _system The trigger's Gazebo system.
      public: void SetSystem(std::shared_ptr<gazebo::System> _system);

      /// \brief Get the trigger name.
      /// \return The trigger's name.
      public: std::string Name() const;

      /// \brief Set the trigger name.
      /// \param[in] _name The trigger name.
      public: void SetName(const std::string &_name);

      /// \brief Get the trigger type.
      /// \return The trigger type
      public: TriggerType Type() const;

      /// \brief Set the trigger type
      /// \param[in] _type The trigger type.
      public: void SetType(const TriggerType &_type);

      /// \brief Private data pointer.
      IGN_UTILS_IMPL_PTR(dataPtr)

    };
  }
}
#endif
