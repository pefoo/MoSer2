configure_file("${ETC_DIR}/mem_charts.gp" "${CMAKE_BINARY_DIR}/mem_charts.gp" COPYONLY)

add_library(memoryplugin SHARED memoryplugin.cpp memorypluginprocessors.cpp "${CMAKE_BINARY_DIR}/mem_charts.gp")

target_link_libraries(memoryplugin MeasurementPluginBase DataProcessorHelper)

install(TARGETS memoryplugin
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
