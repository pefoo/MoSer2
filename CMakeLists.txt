cmake_minimum_required(VERSION 3.2)

project(MoSer2 LANGUAGES CXX VERSION 0.1.0)

#
# Load modules from cmake dir
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#
# Options
#
option(ENABLE_IDE_SUPPORT "Add headers as custom target to the project to display them in IDEs" OFF)
option(BUILD_REPORTER_TESTS "Build the report generator as a library and its tests" OFF)

#
# includes
#
# Required for default install dirs
include(GNUInstallDirs)

#
# cmake / make flags
#
set(CMAKE_CXX_STANDARD 17)
# TODO Remove the coverage flag for certain build types
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
add_definitions(-DTESTDATA_DIR="${PROJECT_SOURCE_DIR}/test/testdata/")

# Copy logger config to runtime dir
configure_file("${ETC_DIR}/logger.conf" "${CMAKE_BINARY_DIR}/logger.conf" COPYONLY)
# Copy the application config to runtime dir 
configure_file("${ETC_DIR}/default_settings.conf" "${CMAKE_BINARY_DIR}/moser2.conf" COPYONLY)

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
# A cpplint wrapper. Calls cpplint and prints the results
add_custom_target(cpplint ${PROJECT_SOURCE_DIR}/tools/run_cpp_lint.sh ${PROJECT_SOURCE_DIR})
# Code covare using lcov. Note that you have to run the unit tests before calling this
# Since lcov gets confused with old data, remove all coverage files when done. Thus, running unit tests before executing this again is required
add_custom_target(coverage
  COMMAND lcov -q -d . -c -o lcov_full.info
  COMMAND lcov -q -e lcov_full.info '${PROJECT_SOURCE_DIR}/include/*' '${PROJECT_SOURCE_DIR}/src/*' -o lcov_reduced.info
  COMMAND genhtml -q -o lcov-html lcov_reduced.info
  COMMAND lcov -l lcov_reduced.info
  COMMAND lcov -d . -z)

#
# include sub projects
# TODO This might suck !?!
include_directories(
  SYSTEM
  ./extern
)
include_directories(
  ./include
  ./src
)


add_subdirectory(./src/moser2)
add_subdirectory(./src/reporter)
add_subdirectory(./src/monitoringpluginbase)
add_subdirectory(./src/monitoringplugins/cpuplugin)
add_subdirectory(./src/monitoringplugins/loadplugin)
add_subdirectory(./src/utility)
add_subdirectory(./src/settingsprovider)
add_subdirectory(./src/persistenceservice)
# TODO Use ExternalProject for externals (see https://cmake.org/cmake/help/latest/module/ExternalProject.html)
add_subdirectory(./extern/pluginmanager)
add_subdirectory(./extern/events)
add_subdirectory(./extern/easyloggingpp-9.96.5)
add_subdirectory(./include/imonitoringplugin)
add_subdirectory(./test/utility)
add_subdirectory(./test/settingsprovider)
add_subdirectory(./test/persistenceservice)
add_subdirectory(./test/reporter)
