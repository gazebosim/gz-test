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
#ifndef IGNITION_TEST_SCENARIO_HH_
#define IGNITION_TEST_SCENARIO_HH_

#include <ignition/utils/ImplPtr.hh>
#include <ignition/gazebo/ServerConfig.hh>

#include "ignition/test/config.hh"
#include "Test.hh"

namespace ignition
{
  namespace test
  {
    // Inline bracket to help doxygen filtering.
    inline namespace IGNITION_TEST_VERSION_NAMESPACE {
    class Scenario
    {
      /// \brief Default constructor.
      public: Scenario();

      /// \brief Load a scenario file.
      /// \param[in] _filename The scenario filename
      /// \return True if the file was loaded successfully.
      public: bool Load(const std::string &_filename);

      /// \brief Execute the loaded scenario.
      public: void Run();

      /// \brief Get the scenario name.
      /// \return The scenario name
      public: std::string Name() const;

      /// \brief Set the scenario name.
      /// \param[in] _name The scenario name
      public: void SetName(const std::string &_name);

      /// \brief Get the scenario description.
      /// \return The description
      public: std::string Description() const;

      /// \brief Set the scenario description.
      /// \param[in] _description The description
      public: void SetDescription(const std::string &_description);

      /// \brief Get the world filename.
      /// \return The SDF world filename
      public: std::string WorldFilename() const;

      /// \brief Set the world filename.
      /// \param[in] _worldFilename The SDF world filename
      public: void SetWorldFilename(const std::string &_worldFilename);

      /// \brief Get the models.
      /// \return The models specified in the configuration.
      public: std::vector<sdf::Model> Models() const;

      /// \brief Set the models.
      /// \param[in] _model The models to run in simulation.
      public: void SetModels(const std::vector<sdf::Model> &_model);

      /// \brief Get the tests to run.
      /// \return The tests to run.
      public: std::vector<Test> Tests() const;

      /// \brief Set the tests to run.
      /// \param[in] _tests The tests to run.
      public: void SetTests(const std::vector<Test> &_tests);

      /// \brief Get the server config.
      /// \return The server configuration.
      public: gazebo::ServerConfig ServerConfig() const;

      /// \brief Set the server config.
      /// \param[in] _config The server configuration.
      public: void SetServerConfig(const gazebo::ServerConfig &_config);

      /// \brief Private data pointer.
      IGN_UTILS_IMPL_PTR(dataPtr)
    };
    }
  }
}
#endif
