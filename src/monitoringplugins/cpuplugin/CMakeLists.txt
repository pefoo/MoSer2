configure_file("${ETC_DIR}/cpu_charts.gp" "${CMAKE_BINARY_DIR}/cpu_charts.gp" COPYONLY)

add_library(cpuplugin SHARED cpuplugin.cpp cpupluginprocessors.cpp "${CMAKE_BINARY_DIR}/cpu_charts.gp")
target_link_libraries(cpuplugin MeasurementPluginBase DataProcessorHelper)

install(TARGETS cpuplugin
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(FILES
  "${CMAKE_BINARY_DIR}/cpu_charts.gp"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
