cmake_minimum_required(VERSION 3.2)

project(MoSer2 LANGUAGES CXX VERSION 0.1.0)

#
# Options
#
option(ENABLE_IDE_SUPPORT "Add headers as custom target to the project to display them in IDEs" OFF)

#
# includes
#
# Required for default install dirs
include(GNUInstallDirs)

#
# cmake / make flags
#
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

#
# set output dirs
#
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/runtime)
set(CMAKE_ARCHIVE_DIR ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# other directories
set(ETC_DIR "./etc")

# Copy logger config to runtime dir
configure_file("${ETC_DIR}/logger.conf" "${CMAKE_BINARY_DIR}/logger.conf" COPYONLY)

#
# Testing
#
enable_testing()

#
# IDE support
#
if (ENABLE_IDE_SUPPORT)
    FILE(GLOB_RECURSE HEADERS "*.hpp")
    FILE(GLOB_RECURSE GNUPLOT "*.gnuplot")
    add_custom_target(header SOURCES ${HEADERS})
    add_custom_target(scripts SOURCES ${GNUPLOT})
endif()

#
# Custom targets
#
add_custom_target(cpplint ${PROJECT_SOURCE_DIR}/tools/run_cpp_lint.sh ${PROJECT_SOURCE_DIR})

#
# include sub projects
#
include_directories(
  ./extern
  ./include
  ./src
)

add_subdirectory(./src/moser2)
add_subdirectory(./src/monitoringpluginbase)
add_subdirectory(./src/monitoringplugins/cpuplugin)
add_subdirectory(./src/utility)
add_subdirectory(./extern/pluginmanager)
add_subdirectory(./extern/events)
add_subdirectory(./extern/easyloggingpp-9.96.5)
add_subdirectory(./include/imonitoringplugin)
