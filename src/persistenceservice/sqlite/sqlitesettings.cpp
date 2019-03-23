#include "persistenceservice/sqlite/sqlitesettings.hpp"

persistenceservice::sqlite::SqliteSettings::SqliteSettings()
    : persistenceservice::AdapterSettings(
          persistenceservice::AdapterTypes::sqlite) {}
