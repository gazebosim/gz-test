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

#include <chrono>
#include <iostream>
#include <ignition/gazebo/Util.hh>
#include <ignition/gazebo/Server.hh>
#include <ignition/utils/cli/CLI.hpp>

#include "ignition/test/config.hh"

#include "Scenario.hh"

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

  Scenario scenario;
  if (!scenario.Load(scenarioFilename))
  {
    ignerr << "Failed to load the scenario file[" << scenarioFilename << "]\n";
    return -1;
  }

  scenario.Run();

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
  return 0;
}
