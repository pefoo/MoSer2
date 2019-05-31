set(SOURCES
)
# Maybe this could become a interface library. who knows...
ADD_LIBRARY(Constants STATIC ${SOURCES})
target_link_libraries(Constants SettingsProvider)

target_include_directories(Constants
PUBLIC
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/constants>
PRIVATE
  ${PROJECT_SOURCE_DIR}/src/constants
)

install(TARGETS Constants
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/constants DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
