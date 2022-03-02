#ifndef IGNITION_TEST_TEST_HH_
#define IGNITION_TEST_TEST_HH_

#include <yaml-cpp/yaml.h>
#include <ignition/utils/ImplPtr.hh>

#include <ignition/gazebo/Server.hh>
#include <ignition/gazebo/ServerConfig.hh>

namespace ignition
{
  namespace test
  {
  class Test
  {
    /// \brief Default constructor.
    public: Test();

    /// \brief Load a test.
    /// \param[in] _node The YAML node containing test information
    /// \return True if the test was loaded successfully.
    public: bool Load(const YAML::Node &_node);

    public: void AddTriggersToServer(gazebo::Server &_server);

    /// \brief Private data pointer.
    IGN_UTILS_IMPL_PTR(dataPtr)
  };
  }
}

#endif
