set(EXEC_NAME MoSer2)

set(SOURCES
  main.cpp
  monitoringserver.cpp
  plugin/plugincontroller.cpp
  plugin/pluginfacade.cpp
)

add_executable(${EXEC_NAME} ${SOURCES})
target_link_libraries(${EXEC_NAME} Event)
target_link_libraries(${EXEC_NAME} PluginLoader ${CMAKE_DL_LIBS})
target_link_libraries(${EXEC_NAME} easyloggingpp)
target_link_libraries(${EXEC_NAME} IMonitorPlugin)
target_link_libraries(${EXEC_NAME} Utility)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${EXEC_NAME} Threads::Threads)
