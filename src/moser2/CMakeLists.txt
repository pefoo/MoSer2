set(EXEC_NAME MoSer2)

set(SOURCES
  main.cpp
  monitoringserver.cpp
  plugin/plugincontroller.cpp
)

add_executable(${EXEC_NAME} ${SOURCES})
target_link_libraries(${EXEC_NAME} Event)
target_link_libraries(${EXEC_NAME} PluginLoader ${CMAKE_DL_LIBS})
target_link_libraries(${EXEC_NAME} easyloggingpp)
target_link_libraries(${EXEC_NAME} IMonitorPlugin)
