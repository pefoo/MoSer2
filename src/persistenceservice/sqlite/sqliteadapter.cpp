#include "persistenceservice/sqlite/sqliteadapter.hpp"
#include "persistenceservice/sqlite/sqlitesettings.hpp"

persistenceservice::sqlite::SqliteAdapter::SqliteAdapter(
    AdapterSettings *adapter_settings) {
  auto settings = dynamic_cast<persistenceservice::sqlite::SqliteSettings *>(
      adapter_settings);
}

persistenceservice::sqlite::SqliteAdapter::~SqliteAdapter() = default;

bool persistenceservice::sqlite::SqliteAdapter::Store(
    const imonitorplugin::PluginData &) const {}
