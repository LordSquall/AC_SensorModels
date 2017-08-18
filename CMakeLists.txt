cmake_minimum_required (VERSION 2.6)
project (AC_SensorModels)

# The Version Number
set (AC_SensorModels_VERSION_MAJOR 1)
set (AC_SensorModels_VERSION_MINOR 0)

# configure a header file to pass some of the Cmake settings to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/src/ac_sensormodelsconfig.h.in"
  "${PROJECT_BINARY_DIR}/ac_sensormodelsconfig.h"
)

# add the binary tree to the search path for include files
# so that we will find ElementaEngineConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(AC_SensorModels
src/main.cpp
)