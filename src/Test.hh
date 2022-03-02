/*
 * Copyright (C) 2022 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
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
