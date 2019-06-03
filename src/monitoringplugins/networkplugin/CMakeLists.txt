configure_file("${ETC_DIR}/networkplugin_charts.gp" "${CMAKE_BINARY_DIR}/networkplugin_charts.gp" COPYONLY)
configure_file("${ETC_DIR}/NetworkPlugin.conf" "${CMAKE_BINARY_DIR}/NetworkPlugin.conf" COPYONLY)

add_library(NetworkPlugin SHARED networkplugin.cpp networkpluginprocessors.cpp "${CMAKE_BINARY_DIR}/networkplugin_charts.gp" "${CMAKE_BINARY_DIR}/NetworkPlugin.conf")
target_link_libraries(NetworkPlugin MeasurementPluginBase DataProcessorHelper)

install(TARGETS NetworkPlugin
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(FILES
  "${CMAKE_BINARY_DIR}/NetworkPlugin.conf"
  "${CMAKE_BINARY_DIR}/networkplugin_charts.gp.gp"
  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
