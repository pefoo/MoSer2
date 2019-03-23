set(SOURCES
  ./adapterfactory.cpp
  ./adaptersettings.cpp
  ./sqlite/sqliteadapter.cpp
  ./sqlite/sqlitesettings.cpp
)

ADD_LIBRARY(PersistenceService STATIC ${SOURCES})
target_link_libraries(PersistenceService IMonitorPlugin easyloggingpp)

