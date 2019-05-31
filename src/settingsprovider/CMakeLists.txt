set(SOURCES
  ./settingsprovider.cpp
  settingidentifier.cpp
  settingsfactory.cpp
)

ADD_LIBRARY(SettingsProvider STATIC ${SOURCES})
target_link_libraries(SettingsProvider easyloggingpp Utility stdc++fs)

target_include_directories(SettingsProvider
PUBLIC
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/settingsprovider>
PRIVATE
  ${PROJECT_SOURCE_DIR}/src/settingsprovider
)

install(TARGETS SettingsProvider
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/settingsprovider DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
