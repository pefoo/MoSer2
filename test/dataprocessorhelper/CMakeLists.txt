set(SOURCES
  main.cpp
  base64encoder_test.cpp
  gnuplotparameterdict_test.cpp
  gnuplotwrapper_test.cpp
)

add_executable(dataprocessorhelper_unit_test ${SOURCES})
target_link_libraries(dataprocessorhelper_unit_test DataProcessorHelper stdc++fs)
add_test(NAME DataProcessorHelper_Test COMMAND "${CMAKE_BINARY_DIR}/dataprocessorhelper_unit_test" "-r junit" "-o ${CMAKE_BINARY_DIR}/dataprocessorhelper_test_result.xml")
