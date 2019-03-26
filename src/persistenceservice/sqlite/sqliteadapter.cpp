#include "persistenceservice/sqlite/sqliteadapter.hpp"
#include <sqlite3.h>
#include <exception>
#include <string>
#include "persistenceservice/sqlite/sqlitesettings.hpp"

persistenceservice::sqlite::SqliteAdapter::SqliteAdapter(
    AdapterSettings *adapter_settings) {
  auto settings = dynamic_cast<persistenceservice::sqlite::SqliteSettings *>(
      adapter_settings);
  auto rc = sqlite3_open(settings->file_name().c_str(), &this->db_);
  this->ThrowIfBadCall(
      rc, "Open",
      "Failed to open the sqlite3 database at " + settings->file_name());
}

persistenceservice::sqlite::SqliteAdapter::~SqliteAdapter() {
  if (this->db_) {
    sqlite3_close(this->db_);
    this->db_ = nullptr;
  }
}

bool persistenceservice::sqlite::SqliteAdapter::Store(
    const imonitorplugin::PluginData &) const {}

void persistenceservice::sqlite::SqliteAdapter::ThrowIfBadCall(
    int rc, const std::string &action,
    const std::string &execution_info) const {
  if (rc != SQLITE_OK) {
    throw std::runtime_error("Action [" + action +
                             "] failed with return code " + std::to_string(rc) +
                             "! " + execution_info +
                             " Error: " + sqlite3_errmsg(this->db_));
  }
}
