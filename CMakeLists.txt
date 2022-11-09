cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

#============================================================================
# Initialize the project
#============================================================================
project(gz-test1 VERSION 0.1.0)

#============================================================================
# Find gz-cmake
#============================================================================
# If you get an error at this line, you need to install gz-cmake
find_package(gz-cmake3 REQUIRED)

#============================================================================
# Configure the project
#============================================================================
set(c++standard 17)
gz_configure_project(VERSION_SUFFIX)

#============================================================================
# Set project-specific options
#============================================================================

option(
  GZ_TEST_VENDOR_CLI11
  "If true, use the vendored version of CLI11, otherwise use an external one"
  true)

#============================================================================
# Search for project-specific dependencies
#============================================================================
if(NOT GZ_TEST_VENDOR_CLI11)
  # PKGCONFIG_IGNORE can be removed once pkg-config support in CLI11 (see
  # https://github.com/CLIUtils/CLI11/pull/523) is released
  gz_find_package(CLI11 REQUIRED_BY cli PKGCONFIG_IGNORE)
endif()

#--------------------------------------
# Find gz-utils
gz_find_package(gz-utils2 REQUIRED COMPONENTS cli)
set(GZ_UTILS_VER ${gz-utils1_VERSION_MAJOR})

#--------------------------------------
# Find gz-fuel_tools
gz_find_package(gz-fuel_tools8 REQUIRED)
set(GZ_FUEL_TOOLS_VER ${gz-fuel_tools7_VERSION_MAJOR})

#--------------------------------------
# Find gz-sim
gz_find_package(gz-sim7 REQUIRED)
set(GZ_SIM_VER ${gz-sim7_VERSION_MAJOR})

#--------------------------------------
# Find gz-transport
gz_find_package(gz-transport12 REQUIRED)
set(GZ_TRANSPORT_MAJOR_VER ${gz-transport12_VERSION_MAJOR})

#--------------------------------------
# Find gz-msgs
gz_find_package(gz-msgs9 REQUIRED)
set(GZ_MSGS_MAJOR_VER ${gz-msgs9_VERSION_MAJOR})

find_package(yaml-cpp REQUIRED)

gz_pkg_check_modules(websockets libwebsockets)
if (NOT websockets_FOUND)
  gz_build_warning("Unable to find libwebsockets. The websocket_server plugin will not be built.")
endif()

#--------------------------------------
# Find prerequistes for backward
include(src/vendor/BackwardConfig.cmake)

#============================================================================
# Configure the build
#============================================================================
gz_configure_build(QUIT_IF_BUILD_ERRORS COMPONENTS cli)

#============================================================================
# Create package information
#============================================================================
gz_create_packages()

#============================================================================
# Configure documentation
#============================================================================
configure_file(${CMAKE_SOURCE_DIR}/api.md.in ${CMAKE_BINARY_DIR}/api.md)
configure_file(${CMAKE_SOURCE_DIR}/tutorials.md.in ${CMAKE_BINARY_DIR}/tutorials.md)

gz_create_docs(
  API_MAINPAGE_MD "${CMAKE_BINARY_DIR}/api.md"
  TUTORIALS_MAINPAGE_MD "${CMAKE_BINARY_DIR}/tutorials.md")
