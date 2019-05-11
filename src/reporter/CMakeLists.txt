set(EXEC_NAME MoSerep)

set(SOURCES
  main.cpp
  templateprocessor/templateprocessor.cpp
  templateprocessor/templatetoken.cpp
)

set(LIBRARIES
  easyloggingpp
  IMonitorPlugin
  Utility
  PersistenceService
  SettingsProvider
  stdc++fs
)

add_executable(${EXEC_NAME} ${SOURCES})

if(BUILD_REPORTER_TESTS)
  add_library(MoSerepLib ${SOURCES})
  target_link_libraries(MoSerepLib ${LIBRARIES})
endif()

target_link_libraries(${EXEC_NAME} ${LIBRARIES})
