cmake_minimum_required(VERSION 3.2)

project(MoSer2 LANGUAGES CXX VERSION 0.1.0)

#
# Load modules from cmake dir
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#
# Options
#
option(BUILD_TESTS "Build the unit tests" OFF)

#
# includes
#
# Required for default install dirs
include(GNUInstallDirs)

#
# cmake / make flags
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra -Wpedantic")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

#
# Custom targets
#
add_custom_target(coverage
  COMMAND lcov -q -d . -z
  COMMAND lcov -q -c -i -d . -o base.info
  COMMAND ctest --output-on-failure
  COMMAND lcov -q -d . -c -o coverage.info
  COMMAND lcov -q -a base.info -a coverage.info -o total.info
  COMMAND lcov -Q -e total.info -o filtered_coverage.info '${PROJECT_SOURCE_DIR}/include/*' '${PROJECT_SOURCE_DIR}/src/*'
  COMMAND lcov -l filtered_coverage.info
  COMMAND genhtml -q -o lcov-html filtered_coverage.info)

#
# set output dirs
#
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/runtime)
set(CMAKE_ARCHIVE_DIR ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# other directories
set(ETC_DIR "${PROJECT_SOURCE_DIR}/etc/")
set(EXTERNAL_DIR "${PROJECT_SOURCE_DIR}/extern/")
add_definitions(-DTESTDATA_DIR="${PROJECT_SOURCE_DIR}/test/testdata/")
add_definitions(-DLIBRARY_DIR="${CMAKE_BINARY_DIR}")

include_directories(
  SYSTEM
  ./extern
)
# Dont like it but declaring these include dirs Iam able to specify included headers including their
# module name
include_directories(
  ./include
  ./src
)


add_subdirectory(./src/moser2)
add_subdirectory(./src/reporter)
add_subdirectory(./src/monitoringpluginbase)
add_subdirectory(./src/monitoringplugins/cpuplugin)
add_subdirectory(./src/monitoringplugins/loadplugin)
add_subdirectory(./src/monitoringplugins/memoryplugin)
add_subdirectory(./src/monitoringplugins/diskplugin)
add_subdirectory(./src/monitoringplugins/networkplugin)
add_subdirectory(./src/monitoringplugins/logwatcherplugin)
add_subdirectory(./src/utility)
add_subdirectory(./src/settingsprovider)
add_subdirectory(./src/persistenceservice)
add_subdirectory(./src/constants)
add_subdirectory(./src/core)
add_subdirectory(./src/dataprocessorhelper)
add_subdirectory(./include/pluginmanager)
add_subdirectory(./include/imonitoringplugin)
if (BUILD_TESTS)
  enable_testing()
  add_subdirectory(./test/pluginmanager)
  add_subdirectory(./test/utility)
  add_subdirectory(./test/settingsprovider)
  add_subdirectory(./test/persistenceservice)
  add_subdirectory(./test/reporter)
  add_subdirectory(./test/dataprocessorhelper)
  add_subdirectory(./test/monitoringplugins)
endif()
