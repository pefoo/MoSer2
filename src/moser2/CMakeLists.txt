set(EXEC_NAME MoSer2)

set(SOURCES
  main.cpp
  monitoringserver.cpp
  plugin/plugincontroller.cpp
  plugin/pluginfacade.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SETTINGS_FILE "${CMAKE_BINARY_DIR}/moser2.conf")
configure_file("${ETC_DIR}/default_settings.conf" "${SETTINGS_FILE}" COPYONLY)

add_executable(${EXEC_NAME} ${SOURCES} ${SETTINGS_FILE})
target_link_libraries(${EXEC_NAME}
  PluginLoader
  ${CMAKE_DL_LIBS}
  easyloggingpp
  IMonitorPlugin
  Utility
  PersistenceService
  SettingsProvider
  stdc++fs
  Constants
  Core
  Threads::Threads
)
