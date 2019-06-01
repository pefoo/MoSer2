set(SOURCES
  ./threading/callbacktimer.cpp
  ./filesystem/fileaccesshelper.cpp
  ./datastructure/anyhelper.cpp
  ${EXTERNAL_DIR}/easyloggingpp-9.96.5/easylogging++.cc
)

ADD_LIBRARY(Utility STATIC ${SOURCES})
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Utility
  Threads::Threads
  stdc++fs
)

target_include_directories(Utility
PUBLIC
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/utility>
PRIVATE
  ${PROJECT_SOURCE_DIR}/src/utility
)
install(TARGETS Utility
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/utility DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
