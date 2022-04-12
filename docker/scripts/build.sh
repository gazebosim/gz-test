#!/bin/bash

./scripts/build_ign.sh ignitionrobotics ign-cmake ign-cmake2
./scripts/build_ign.sh ignitionrobotics ign-tools ign-tools1
./scripts/build_ign.sh ignitionrobotics ign-math ign-math6
./scripts/build_ign.sh ignitionrobotics ign-utils ign-utils1
./scripts/build_ign.sh ignitionrobotics ign-common ign-common4
./scripts/build_ign.sh ignitionrobotics ign-msgs ign-msgs8
./scripts/build_ign.sh ignitionrobotics ign-transport ign-transport11
./scripts/build_ign.sh ignitionrobotics ign-fuel-tools ign-fuel-tools7
./scripts/build_ign.sh ignitionrobotics sdformat plugin_equality
./scripts/build_ign.sh ignitionrobotics ign-plugin ign-plugin1
./scripts/build_ign.sh ignitionrobotics ign-physics ign-physics5
./scripts/build_ign.sh ignitionrobotics ign-rendering ign-rendering6
./scripts/build_ign.sh ignitionrobotics ign-gui ign-gui6 
./scripts/build_ign.sh ignitionrobotics ign-sensors ign-sensors6
./scripts/build_ign.sh ignitionrobotics ign-gazebo sensor_plugin_autoload
# ./scripts/build_ign.sh ignitionrobotics ign-launch ign-launch5
# ./scripts/build_ign.sh nkoenig ign-test main
