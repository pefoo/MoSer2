configure_file("${ETC_DIR}/LogWatcherPlugin.conf" "${CMAKE_BINARY_DIR}/LogWatcherPlugin.conf" COPYONLY)
add_library(LogWatcherPlugin SHARED logwatcherplugin.cpp logwatcherpluginprocessors.cpp "${CMAKE_BINARY_DIR}/logwatcherplugin_charts.gp")
target_link_libraries(LogWatcherPlugin MeasurementPluginBase DataProcessorHelper stdc++fs)

install(TARGETS LogWatcherPlugin
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(FILES
  "${CMAKE_BINARY_DIR}/LogWatcherPlugin.conf"
  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
