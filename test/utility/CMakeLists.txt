set(SOURCES
  main.cpp
  threading/callbacktimer_test.cpp
)

add_executable(utility_unit_test ${SOURCES})
target_link_libraries(utility_unit_test Utility)
add_test(Utility_Test "${CMAKE_BINARY_DIR}/utility_unit_test")
