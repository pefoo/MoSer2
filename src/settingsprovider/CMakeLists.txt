set(SOURCES
  ./settingsprovider.cpp
  settingidentifier.cpp
  settingsfactory.cpp
)

ADD_LIBRARY(SettingsProvider STATIC ${SOURCES})
target_link_libraries(SettingsProvider easyloggingpp Utility stdc++fs)

