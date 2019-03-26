set(SOURCES
  main.cpp
  ./sqlite3adapter_test.cpp
)

add_executable(persistenceservice_unit_test ${SOURCES})
target_link_libraries(persistenceservice_unit_test PersistenceService)
add_test(NAME PersistenceService_Test COMMAND "${CMAKE_BINARY_DIR}/persistenceservice_unit_test" "-r junit" "-o ${CMAKE_BINARY_DIR}/persistenceservice_test_result.xml")
