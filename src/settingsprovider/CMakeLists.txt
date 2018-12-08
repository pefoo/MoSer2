set(SOURCES
  ./settingsprovider.cpp
  ./converters.cpp
  settingidentifier.cpp
)

ADD_LIBRARY(SettingsProvider STATIC ${SOURCES})
target_link_libraries(SettingsProvider easyloggingpp Utility)

