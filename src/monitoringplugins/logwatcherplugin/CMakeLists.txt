configure_file("${ETC_DIR}/LogWatcherPlugin.conf" "${CMAKE_BINARY_DIR}/LogWatcherPlugin.conf" COPYONLY)
configure_file("${ETC_DIR}/logwatcher_charts.gp" "${CMAKE_BINARY_DIR}/logwatcher_charts.gp" COPYONLY)

add_library(LogWatcherPlugin SHARED logwatcherplugin.cpp logwatcherpluginprocessors.cpp)
target_link_libraries(LogWatcherPlugin MeasurementPluginBase DataProcessorHelper stdc++fs)

install(TARGETS LogWatcherPlugin
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(FILES
  "${CMAKE_BINARY_DIR}/LogWatcherPlugin.conf"
   "${CMAKE_BINARY_DIR}/logwatcher_charts.gp"
  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
