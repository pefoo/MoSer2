set(SOURCES
  ./adapterfactory.cpp
  ./adaptersettings.cpp
  ./sqlite/sqliteadapter.cpp
  ./sqlite/sqlitesettings.cpp
  ./sqlite/queryhelper.cpp
  ./persistencerunner.cpp
)

find_package(sqlite3 REQUIRED)
include_directories(${SQLITE3_INCLUDE_DIRS})

ADD_LIBRARY(PersistenceService STATIC ${SOURCES})
target_link_libraries(PersistenceService IMonitorPlugin easyloggingpp ${SQLITE3_LIBRARIES} Utility)

target_include_directories(PersistenceService
PUBLIC
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/persistenceservice>
PRIVATE
  ${PROJECT_SOURCE_DIR}/src/persistenceservice
)

install(TARGETS PersistenceService
  ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
  RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/persistenceservice DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
