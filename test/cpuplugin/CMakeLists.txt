set(SOURCES
  main.cpp
  cpuplugin_test.cpp
)

add_executable(cpuplugin_unit_test ${SOURCES})
target_link_libraries(cpuplugin_unit_test cpuplugin)
add_test(NAME CpuPlugin_Test COMMAND "${CMAKE_BINARY_DIR}/cpuplugin_unit_test" "-r junit" "-o ${CMAKE_BINARY_DIR}/cpuplugin_test_result.xml")
