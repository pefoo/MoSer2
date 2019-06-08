set(SOURCES
  main.cpp
  configurationinjector.cpp
  cpuplugin_test.cpp
  diskplugin_test.cpp
  loadplugin_test.cpp
  memoryplugin_test.cpp
  networkplugin_test.cpp
)
add_definitions(-DLINKED_PLUGIN)
add_executable(monitoringplugins_unit_test ${SOURCES})
target_link_libraries(monitoringplugins_unit_test cpuplugin diskplugin loadplugin memoryplugin NetworkPlugin)
add_test(NAME MonitoringPlugins_Test COMMAND "${CMAKE_BINARY_DIR}/monitoringplugins_unit_test" "-r junit" "-o ${CMAKE_BINARY_DIR}/monitoringplugins_test_result.xml")
