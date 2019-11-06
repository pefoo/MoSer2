set(SOURCES
  main.cpp
  base64encoder_test.cpp
  gnuplotparameterdict_test.cpp
)

if (INCLUDE_GNUPLOT_DEPENDENT_TESTS)
  set(SOURCES ${SOURCES} gnuplotwrapper_test.cpp)
endif()

add_executable(dataprocessorhelper_unit_test ${SOURCES})
target_link_libraries(dataprocessorhelper_unit_test DataProcessorHelper stdc++fs IMonitorPlugin)
add_test(NAME DataProcessorHelper_Test COMMAND "${CMAKE_BINARY_DIR}/dataprocessorhelper_unit_test")
