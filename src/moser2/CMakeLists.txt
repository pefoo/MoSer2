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
set(LOGGER_CONFIG_FILE "${CMAKE_BINARY_DIR}/logger.conf")

if (NOT EXISTS "${SETTINGS_FILE}")
  configure_file("${ETC_DIR}/default_settings.conf" "${SETTINGS_FILE}" COPYONLY)
endif()

if (NOT EXISTS "${LOGGER_CONFIG_FILE}")
  configure_file("${ETC_DIR}/logger.conf" "${LOGGER_CONFIG_FILE}" COPYONLY)
endif()

add_executable(${EXEC_NAME} ${SOURCES} ${SETTINGS_FILE} ${LOGGER_CONFIG_FILE})
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
