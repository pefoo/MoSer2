set(EXEC_NAME MoSerep)

set(SOURCES
  main.cpp
)

add_executable(${EXEC_NAME} ${SOURCES})
target_link_libraries(${EXEC_NAME} easyloggingpp)
target_link_libraries(${EXEC_NAME} IMonitorPlugin)
target_link_libraries(${EXEC_NAME} Utility)
target_link_libraries(${EXEC_NAME} PersistenceService)
target_link_libraries(${EXEC_NAME} SettingsProvider)
target_link_libraries(${EXEC_NAME} stdc++fs)
