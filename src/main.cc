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
#include <yaml-cpp/yaml.h>

#include <chrono>
#include <iostream>
#include <sdf/Model.hh>
#include <sdf/Root.hh>
#include <sdf/World.hh>
#include <ignition/gazebo/Util.hh>
#include <ignition/gazebo/Server.hh>
#include <ignition/utils/cli/CLI.hpp>
#include <ignition/fuel_tools/Interface.hh>

#include "ignition/test/config.hh"
#include "TimeTrigger.hh"

using namespace std::chrono_literals;
using namespace ignition;
using namespace test;

int main(int argc, char **argv)
{
  CLI::App app{"Test runner"};
  app.set_help_all_flag("--help-all", "Show all help");
  app.add_flag_callback("-v,--version", [](){
      std::cout << IGNITION_TEST_VERSION_FULL << std::endl;
      throw CLI::Success();
  });

  std::string scenarioFilename = "";
  app.add_option("-s,--scenario-file",
      scenarioFilename, "Specify the scenario file")
    ->required()
    ->check(CLI::ExistingFile);

  CLI11_PARSE(app, argc, argv);

  // Try to parse the scenario file.
  YAML::Node config;
  try
  {
    config = YAML::LoadFile(scenarioFilename);
  }
  catch (YAML::ParserException &_e)
  {
    std::cerr << "Invalid scenario format, with error: " << _e.msg << "\n";
    return -1;
  }

  std::string worldFilename = "";

  // if (config["name"])
  //   std::cout << "Name: " << config["name"].as<std::string>() << "\n";

  // if (config["description"])
  //   std::cout << "Description: " << config["description"].as<std::string>() << "\n";

  std::vector<sdf::Model> models;
  // Get the world file
  if (config["configuration"])
  {
    YAML::Node configNode = config["configuration"];
    if (configNode["world"])
      worldFilename = configNode["world"].as<std::string>();
    if (configNode["models"])
    {
      for (YAML::const_iterator it = configNode["models"].begin();
          it!=configNode["models"].end(); ++it)
      {
        sdf::Model model;

        // Get the model's URI
        if ((*it)["uri"])
        {
          std::string uri = (*it)["uri"].as<std::string>();
          std::string modelPath = fuel_tools::fetchResource(uri);
          std::string modelSdfFile = fuel_tools::sdfFromPath(modelPath);
          sdf::Errors errors = model.LoadSdfFile(modelSdfFile);

          for (sdf::Error &err : errors)
            std::cout << err.Message() << std::endl;

          model.SetUri(uri);
        }
        else
        {
          ignerr << "model missing URI, skipping\n";
          continue;
        }

        // Get the model's name
        if ((*it)["name"])
          model.SetName((*it)["name"].as<std::string>());
        else
        {
          ignerr << "model missing name, skipping\n";
          continue;
        }

        // Get the model's pose
        if ((*it)["pose"])
        {
          math::Pose3d pose;
          if ((*it)["pose"]["x"])
            pose.SetX((*it)["pose"]["x"].as<double>());
          if ((*it)["pose"]["y"])
            pose.SetY((*it)["pose"]["y"].as<double>());
          if ((*it)["pose"]["z"])
            pose.SetZ((*it)["pose"]["z"].as<double>());

          math::Vector3d rot = pose.Rot().Euler();

          if ((*it)["pose"]["roll"])
            rot.X((*it)["pose"]["roll"].as<double>());
          if ((*it)["pose"]["pitch"])
            rot.Y((*it)["pose"]["pitch"].as<double>());
          if ((*it)["pose"]["yaw"])
            rot.Z((*it)["pose"]["yaw"].as<double>());

          pose.Rot().Euler(rot);
          model.SetRawPose(pose);
        }

        models.push_back(model);
      }
    }
  }

  std::string worldFilePath = gazebo::resolveSdfWorldFile(worldFilename);
  if (worldFilePath.empty())
  {
    ignerr << "Unable to find world file[" << worldFilename << "]\n";
    return -1;
  }

  gazebo::ServerConfig serverConfig;
  sdf::Root root;
  sdf::Errors errors = root.Load(worldFilePath);
  if (!errors.empty())
  {
    ignerr << "Failed to load SDF world file[" << worldFilePath << "]\n";
    for (const sdf::Error err : errors)
      ignerr << err.Message() << std::endl;

    return -1;
  }

  for (sdf::Model model : models)
    root.WorldByIndex(0)->AddModel(model);

  // Set the world file to load.
  serverConfig.SetSdfRoot(root);

  // 1. Load a scenario file.
  //     a. X Load the world.
  //     b. X Place objects.
  //     c. Apply initial forces and joint positions.
  //     d. Create triggers
  //         - Time-based
  //         - Region-based
  //         - External trigger
  //         - Physics state such as velocity or force threshold.
  // 2. Spawn simulation.
  //    - Support spawning multiple simulation instances simulatanously
  // 3. Get results, and log simulation state.
  // 4. Print results.
  //
  //
  // On the side:
  //   1. Update documentation.
  //   2. Get bullet working
  //   3. Create alternative to smacc, tinysfm
  //   4. Unified return error codes, with hooks to state logging, and
  //   transport. Should also have boolean evaluation.

  auto server = std::make_unique<gazebo::Server>(serverConfig);

  std::shared_ptr<TimeTrigger> trigger = std::make_shared<TimeTrigger>(10s);
  server->AddSystem(trigger);
  server->Run(true, 20000, false);

  return 0;
}
