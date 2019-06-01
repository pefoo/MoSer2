set(SOURCES
  appsetup.cpp
  settings/settingshelper.cpp
  ${EXTERNAL_DIR}/easyloggingpp-9.96.5/easylogging++.cc
)

ADD_LIBRARY(Core STATIC ${SOURCES})
target_link_libraries(Core Utility stdc++fs Constants SettingsProvider)

target_include_directories(Core
PUBLIC
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/core>
PRIVATE
  ${PROJECT_SOURCE_DIR}/src/core
)

install(TARGETS Core
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/core DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
