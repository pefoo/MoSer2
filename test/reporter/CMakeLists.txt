set(SOURCES
  main.cpp
  templateprocessor_test.cpp
)

if (BUILD_REPORTER_TESTS)
  add_executable(reporter_unit_test ${SOURCES})
  target_link_libraries(reporter_unit_test MoSerepLib)
  add_test(NAME Reporter_Test COMMAND "${CMAKE_BINARY_DIR}/reporter_unit_test" "-r junit" "-o ${CMAKE_BINARY_DIR}/reporter_test_result.xml")
endif()
