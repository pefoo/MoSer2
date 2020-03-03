set(EXEC_NAME ConfigHelper)

set(SOURCES
  main.cpp
  ${EXTERNAL_DIR}/easyloggingpp-9.96.5/easylogging++.cc
)

set(LIBRARIES
  IMonitorPlugin
  MeasurementPluginBase
  Core
  Utility
  SettingsProvider
  stdc++fs
  Constants
  PluginLoader
  ${CMAKE_DL_LIBS}
)

add_executable(${EXEC_NAME} ${SOURCES})

target_link_libraries(${EXEC_NAME} ${LIBRARIES})

install(TARGETS ${EXEC_NAME}
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
