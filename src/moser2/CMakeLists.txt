set(EXEC_NAME MoSer2)

set(SOURCES
  main.cpp
  monitoringserver.cpp
  plugin/plugincontroller.cpp
  plugin/pluginfacade.cpp
  plugin/inputfileprovider.cpp
  ${EXTERNAL_DIR}/easyloggingpp-9.96.5/easylogging++.cc
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SETTINGS_FILE "${CMAKE_BINARY_DIR}/moser2.conf")
set(LOGGER_CONFIG_FILE "${CMAKE_BINARY_DIR}/logger.conf")

if (NOT EXISTS "${SETTINGS_FILE}")
  if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    configure_file("${ETC_DIR}/debug_settings.conf" "${SETTINGS_FILE}" COPYONLY)
  else()
    configure_file("${ETC_DIR}/default_settings.conf" "${SETTINGS_FILE}" COPYONLY)
  endif()
endif()

if (NOT EXISTS "${LOGGER_CONFIG_FILE}")
  configure_file("${ETC_DIR}/logger.conf" "${LOGGER_CONFIG_FILE}" COPYONLY)
endif()

add_executable(${EXEC_NAME} ${SOURCES} ${SETTINGS_FILE} ${LOGGER_CONFIG_FILE})
target_link_libraries(${EXEC_NAME}
  PluginLoader
  ${CMAKE_DL_LIBS}
  IMonitorPlugin
  Utility
  PersistenceService
  SettingsProvider
  stdc++fs
  Constants
  Core
  Threads::Threads
)

install(TARGETS ${EXEC_NAME}
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(FILES
  ${SETTINGS_FILE}
  ${LOGGER_CONFIG_FILE}
  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
