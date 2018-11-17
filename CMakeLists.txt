cmake_minimum_required(VERSION 3.2)

project(MoSer2 LANGUAGES CXX VERSION 0.1)

#
# includes
#
# Required for default install dirs
include(GNUInstallDirs)

#
# cmake / make flags
#
set(CMAKE_CXX_STANDARD 11)

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
# include sub projects
#
include_directories(./extern)

add_subdirectory(./src/moser2)
add_subdirectory(./extern/pluginmanager)
add_subdirectory(./extern/events)
add_subdirectory(./extern/easyloggingpp-9.96.5)
