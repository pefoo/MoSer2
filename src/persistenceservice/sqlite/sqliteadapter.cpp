#include "persistenceservice/sqlite/sqliteadapter.hpp"
#include <sqlite3.h>
#include <algorithm>
#include <exception>
#include <string>
#include "persistenceservice/sqlite/queryhelper.hpp"
#include "persistenceservice/sqlite/sqlitesettings.hpp"

static int QueryCallback(void *data, int argc, char **argv, char **azColName) {}

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

void persistenceservice::sqlite::SqliteAdapter::Store(
    const imonitorplugin::PluginData &data) {
  char *err_msg;
  if (std::find(this->known_plugins_.begin(), this->known_plugins_.end(),
                data.plugin_name()) == this->known_plugins_.end()) {
    auto query =
        persistenceservice::sqlite::QueryHelper::BuildCreateTableQuery(data);
    int rc = sqlite3_exec(this->db_, query.c_str(), QueryCallback, nullptr,
                          &err_msg);
    this->ThrowIfBadCall(rc, "Create table " + data.plugin_name(), err_msg);
    this->known_plugins_.push_back(data.plugin_name());
  }

  auto query = persistenceservice::sqlite::QueryHelper::BuildInsertQuery(data);
  int rc =
      sqlite3_exec(this->db_, query.c_str(), QueryCallback, nullptr, &err_msg);
  this->ThrowIfBadCall(rc, "Insert into table " + data.plugin_name(), err_msg);
}

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

void persistenceservice::sqlite::SqliteAdapter::ThrowIfBadCall(
    int rc, const std::string &action, char *execution_info) const {
  if (execution_info) {
    this->ThrowIfBadCall(rc, action, execution_info);
  } else {
    this->ThrowIfBadCall(rc, action);
  }
}
