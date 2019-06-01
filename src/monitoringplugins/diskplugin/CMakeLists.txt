configure_file("${ETC_DIR}/disk_charts.gp" "${CMAKE_BINARY_DIR}/disk_charts.gp" COPYONLY)

add_library(diskplugin SHARED diskplugin.cpp diskpluginprocessors.cpp "${CMAKE_BINARY_DIR}/disk_charts.gp")
target_link_libraries(diskplugin MeasurementPluginBase DataProcessorHelper)

install(TARGETS diskplugin
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(FILES
  "${CMAKE_BINARY_DIR}/disk_charts.gp"
  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
