set(SOURCES
  monitorpluginbase.cpp
  pluginexception.cpp
)

ADD_LIBRARY(MeasurementPluginBase STATIC ${SOURCES})
target_link_libraries(MeasurementPluginBase IMonitorPlugin SettingsProvider Utility)
