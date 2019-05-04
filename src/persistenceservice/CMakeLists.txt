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
target_link_libraries(PersistenceService IMonitorPlugin easyloggingpp ${SQLITE3_LIBRARIES})

