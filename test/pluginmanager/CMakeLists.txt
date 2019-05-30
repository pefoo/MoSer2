set(SOURCES
  main.cpp
  pluginloader_test.cpp
  pluginmanager_test.cpp
)

add_subdirectory("./sample_lib")
add_executable(plugin_loader_unit_test ${SOURCES})
target_link_libraries(plugin_loader_unit_test PluginLoader ${CMAKE_DL_LIBS})
add_test(NAME PluginManager_Test COMMAND "${CMAKE_BINARY_DIR}/plugin_loader_unit_test" "-r junit" "-o ${CMAKE_BINARY_DIR}/pluginmanager_test_result.xml")
