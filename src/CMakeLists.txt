add_subdirectory(msgs)
add_subdirectory(websocket_server)

set_source_files_properties(
  ${PROTO_PRIVATE_SRC}
  ${PROTO_PRIVATE_HEADERS}
  PROPERTIES GENERATED TRUE
)

# Suppress compiler warnings in generated protobuf C++ code.
if(NOT MSVC)
  set_source_files_properties(
    ${PROTO_PRIVATE_SRC}
    COMPILE_FLAGS -Wno-unused-parameter
  )
endif()

set (sources
  ProcessManager.cc
  RegionTrigger.cc
  Scenario.cc
  Test.cc
  Trigger.cc
  TimeTrigger.cc
  Util.cc
  websocket_server/WebsocketServer.cc
  ${PROTO_PRIVATE_SRC}
)

# Create the library target
ign_create_core_library(SOURCES ${sources} CXX_STANDARD 17)
target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME}
  PUBLIC
  ignition-gazebo${IGN_GAZEBO_VER}::ignition-gazebo${IGN_GAZEBO_VER}
  ignition-utils${IGN_UTILS_VER}::cli
  ignition-fuel_tools${IGN_FUEL_TOOLS_VER}::ignition-fuel_tools${IGN_FUEL_TOOLS_VER}
  ignition-msgs${IGN_MSGS_MAJOR_VER}::core
  ignition-transport${IGN_TRANSPORT_MAJOR_VER}::core
  ${YAML_CPP_LIBRARIES}
  PRIVATE
  ${websockets_LIBRARIES}
  ${BACKWARD_LIBRARIES}
)
if (UNIX AND NOT APPLE)
  target_link_libraries(${PROJECT_LIBRARY_TARGET_NAME}
    PRIVATE stdc++fs)
endif()

target_compile_definitions(${PROJECT_LIBRARY_TARGET_NAME} PRIVATE ${BACKWARD_DEFINITIONS})

add_executable(ign-test main.cc)
target_include_directories(${PROJECT_LIBRARY_TARGET_NAME}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
add_dependencies(${PROJECT_LIBRARY_TARGET_NAME}
  ignition-test_private_msgs
)
target_link_libraries(ign-test
    ${PROJECT_LIBRARY_TARGET_NAME}
)

install (TARGETS ign-test DESTINATION ${BIN_INSTALL_DIR})
