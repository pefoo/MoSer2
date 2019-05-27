set(EXEC_NAME MoSerep)

set(SOURCES
  main.cpp
  templateprocessor/templateprocessor.cpp
  templateprocessor/templatetoken.cpp
  templateprocessor/templatetokenfactory.cpp
  email/emailsender.cpp
  tokens/datalesstokenfactory.cpp
)

find_package(CURL REQUIRED)
set(LIBRARIES
  easyloggingpp
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

add_executable(${EXEC_NAME} ${SOURCES})
include_directories(${CURL_INCLUDE_DIR})

if(BUILD_REPORTER_TESTS)
  add_library(MoSerepLib ${SOURCES})
  target_link_libraries(MoSerepLib ${LIBRARIES})
endif()

target_link_libraries(${EXEC_NAME} ${LIBRARIES})
