set(SOURCES
  main.cpp
  threading/callbacktimer_test.cpp
  datastructure/circularbuffer_test.cpp
  datastructure/any_test.cpp
  filesystem/fileaccesshelper_test.cpp
)

add_executable(utility_unit_test ${SOURCES})
target_link_libraries(utility_unit_test Utility)
add_test(NAME Utility_Test COMMAND "${CMAKE_BINARY_DIR}/utility_unit_test" "-r junit" "-o ${CMAKE_BINARY_DIR}/utility_test_result.xml")
