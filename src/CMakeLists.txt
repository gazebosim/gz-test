#ign_get_libsources_and_unittests(sources gtest_sources)

#ign_create_core_library(SOURCES ${sources} CXX_STANDARD ${c++standard})

set (sources
  Scenario.cc
  Trigger.cc
  TimeTrigger.cc
)

# Create the library target
ign_create_core_library(SOURCES ${sources} CXX_STANDARD 17)
target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME}
  PUBLIC
  ignition-gazebo${IGN_GAZEBO_VER}::ignition-gazebo${IGN_GAZEBO_VER}
  ignition-utils${IGN_UTILS_VER}::cli
  ignition-fuel_tools${IGN_FUEL_TOOLS_VER}::ignition-fuel_tools${IGN_FUEL_TOOLS_VER}
  ${YAML_CPP_LIBRARIES}
)
if (UNIX AND NOT APPLE)
  target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME}
    PRIVATE stdc++fs)
endif()

add_executable(tester main.cc)
target_link_libraries(tester
    ${PROJECT_LIBRARY_TARGET_NAME}
)
