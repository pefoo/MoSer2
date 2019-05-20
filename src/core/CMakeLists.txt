set(SOURCES
  appsetup.cpp
  settings/settingshelper.cpp
)

ADD_LIBRARY(Core STATIC ${SOURCES})
target_link_libraries(Core easyloggingpp Utility stdc++fs Constants SettingsProvider)
