set(SOURCES
  main.cpp
  templateprocessor_test.cpp
  templatetoken_test.cpp
)

add_executable(reporter_unit_test ${SOURCES})
target_link_libraries(reporter_unit_test MoSerepLib stdc++fs)
add_test(NAME Reporter_Test COMMAND "${CMAKE_BINARY_DIR}/reporter_unit_test" "-r junit" "-o ${CMAKE_BINARY_DIR}/reporter_test_result.xml")
