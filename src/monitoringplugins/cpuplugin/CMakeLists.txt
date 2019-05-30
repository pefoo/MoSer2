configure_file("${ETC_DIR}/cpu_charts.gp" "${CMAKE_BINARY_DIR}/cpu_charts.gp" COPYONLY)

add_library(cpuplugin SHARED cpuplugin.cpp cpupluginprocessors.cpp "${CMAKE_BINARY_DIR}/cpu_charts.gp")
target_link_libraries(cpuplugin MeasurementPluginBase DataProcessorHelper)
