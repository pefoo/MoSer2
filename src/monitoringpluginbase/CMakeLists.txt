set(SOURCES
  monitorpluginbase.cpp
  plugindataprocessor.cpp
  plugindataprocessorcollection.cpp
)

ADD_LIBRARY(MeasurementPluginBase STATIC ${SOURCES})
target_link_libraries(MeasurementPluginBase IMonitorPlugin SettingsProvider Utility stdc++fs)

target_include_directories(MeasurementPluginBase
PUBLIC
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/monitoringpluginbase>
PRIVATE
  ${PROJECT_SOURCE_DIR}/src/monitoringpluginbase
)

install(TARGETS MeasurementPluginBase
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/monitoringpluginbase DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
