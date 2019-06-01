add_library(memoryplugin SHARED memoryplugin.cpp memorypluginprocessors.cpp)

target_link_libraries(memoryplugin MeasurementPluginBase DataProcessorHelper)

install(TARGETS memoryplugin
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
