set(SOURCES
  base64.cpp
  gnuplot/gnuplotparameterdict.cpp
  gnuplot/gnuplotwrapper.cpp
)

ADD_LIBRARY(DataProcessorHelper STATIC ${SOURCES})
target_link_libraries(DataProcessorHelper stdc++fs IMonitorPlugin Utility)

target_include_directories(DataProcessorHelper
PUBLIC
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/dataprocessorhelper>
PRIVATE
  ${PROJECT_SOURCE_DIR}/src/dataprocessorhelper
)

install(TARGETS DataProcessorHelper
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/dataprocessorhelper DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
