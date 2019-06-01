configure_file("${ETC_DIR}/load_charts.gp" "${CMAKE_BINARY_DIR}/load_charts.gp" COPYONLY)
add_library(loadplugin SHARED loadplugin.cpp loadpluginprocessors.cpp "${CMAKE_BINARY_DIR}/load_charts.gp")
target_link_libraries(loadplugin MeasurementPluginBase DataProcessorHelper)

install(TARGETS loadplugin
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(FILES
  "${CMAKE_BINARY_DIR}/load_charts.gp"
  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
