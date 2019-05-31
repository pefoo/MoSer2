add_library(loadplugin SHARED loadplugin.cpp)
target_link_libraries(loadplugin MeasurementPluginBase)

install(TARGETS loadplugin
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
