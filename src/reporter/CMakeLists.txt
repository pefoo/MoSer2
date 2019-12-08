set(EXEC_NAME MoSerep)

set(SOURCES
  main.cpp
  templateprocessor/templateprocessor.cpp
  templateprocessor/templatetoken.cpp
  templateprocessor/templatetokenfactory.cpp
  email/emailsender.cpp
  tokens/datalesstokenfactory.cpp
  helper/processinformationhelper.cpp
  datamodel/processinformation.cpp
  viewmodel/processinformationviewmodel.cpp
  views/processinformationviews.cpp
  ${EXTERNAL_DIR}/easyloggingpp-9.96.5/easylogging++.cc
)

find_package(CURL REQUIRED)
set(LIBRARIES
  IMonitorPlugin
  Utility
  PersistenceService
  SettingsProvider
  stdc++fs
  Constants
  Core
  PluginLoader
  ${CMAKE_DL_LIBS}
  ${CURL_LIBRARIES}
)

set(SETTINGS_FILE "${CMAKE_BINARY_DIR}/moser2.conf")
set(LOGGER_CONFIG_FILE "${CMAKE_BINARY_DIR}/logger.conf")
set(REPORT_TEMPLATE_FILE "${CMAKE_BINARY_DIR}/template.html")

configure_file("${ETC_DIR}/report_template.html" ${REPORT_TEMPLATE_FILE} COPYONLY)
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
include_directories(${CURL_INCLUDE_DIR})

if(BUILD_TESTS)
  add_library(MoSerepLib ${SOURCES})
  target_link_libraries(MoSerepLib ${LIBRARIES})
endif()

target_link_libraries(${EXEC_NAME} ${LIBRARIES})

install(TARGETS ${EXEC_NAME}
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(FILES
  ${SETTINGS_FILE}
  ${LOGGER_CONFIG_FILE}
  ${REPORT_TEMPLATE_FILE}
  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
