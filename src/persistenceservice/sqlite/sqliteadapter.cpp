#include "persistenceservice/sqlite/sqliteadapter.hpp"
#include <sqlite3.h>
#include <algorithm>
#include <exception>
#include <string>
#include <vector>
#include "persistenceservice/sqlite/queryhelper.hpp"
#include "persistenceservice/sqlite/sqlitesettings.hpp"

// TODO is this dummy callback actually required?
static int QueryCallback([[maybe_unused]] void *data, [[maybe_unused]] int argc,
                         [[maybe_unused]] char **argv,
                         [[maybe_unused]] char **col_name) {
  return 0;
}

persistenceservice::sqlite::SqliteAdapter::SqliteAdapter(
    AdapterSettings *adapter_settings)
    : db_(nullptr) {
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
  this->PrepareTable(data);
  this->DoStore(data);
}

void persistenceservice::sqlite::SqliteAdapter::Store(
    const std::vector<imonitorplugin::PluginData>::iterator &begin,
    const std::vector<imonitorplugin::PluginData>::iterator &end) {
  char *err_msg;
  int rc =
      sqlite3_exec(this->db_, "BEGIN TRANSACTION;", nullptr, nullptr, &err_msg);
  this->ThrowIfBadCall(rc, "Begin transaction", err_msg);
  for (auto item = begin; item != end; ++item) {
    this->PrepareTable(*item);
    this->DoStore(*item);
  }
  rc = sqlite3_exec(this->db_, "END TRANSACTION;", nullptr, nullptr, &err_msg);
  this->ThrowIfBadCall(rc, "End transaction", err_msg);
}

std::vector<imonitorplugin::PluginData>
persistenceservice::sqlite::SqliteAdapter::Load(const std::string &plugin_name,
                                                int64_t min_age) {
  sqlite3_stmt *stmt;
  auto query = persistenceservice::sqlite::QueryHelper::BuildSelectQuery(
      plugin_name, min_age);
  int rc = sqlite3_prepare_v2(this->db_, query.c_str(), -1, &stmt, nullptr);
  this->ThrowIfBadCall(rc, "Prepare select for " + plugin_name);

  std::vector<imonitorplugin::PluginData> records;
  // Table rows
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    imonitorplugin::PluginData::data_vector data;
    int64_t ts = 0;
    // Table columns
    for (int col = 0; col < sqlite3_data_count(stmt); ++col) {
      std::string col_name = sqlite3_column_name(stmt, col);

      // Handle special column(s)
      if (col_name ==
          persistenceservice::sqlite::QueryHelper::kSpecialColTimestamp) {
        ts = sqlite3_column_int(stmt, col);
        continue;
      }

      // Handle data columns
      switch (sqlite3_column_type(stmt, col)) {
        case SQLITE_INTEGER: {
          data.push_back({col_name, sqlite3_column_int(stmt, col)});
          break;
        }
        case SQLITE_FLOAT: {
          data.push_back({col_name, sqlite3_column_double(stmt, col)});
          break;
        }
        case SQLITE_TEXT: {
          // Thanks for the unsigned char.
          data.push_back({col_name, std::string(reinterpret_cast<const char *>(
                                        sqlite3_column_text(stmt, col)))});
          break;
        }
        case SQLITE_NULL:
        case SQLITE_BLOB:
          throw std::runtime_error("Not supported sql type.");
      }
    }
    records.emplace_back(imonitorplugin::PluginData(plugin_name, ts, data));
  }
  this->ThrowIfBadCall(rc, "Get table row", sqlite3_errmsg(this->db_),
                       SQLITE_DONE);

  return records;
}

utility::datastructure::Table
persistenceservice::sqlite::SqliteAdapter::LoadTable(
    const std::string &plugin_name, int64_t min_age) {
  sqlite3_stmt *stmt;
  auto query = persistenceservice::sqlite::QueryHelper::BuildSelectQuery(
      plugin_name, min_age);
  int rc = sqlite3_prepare_v2(this->db_, query.c_str(), -1, &stmt, nullptr);
  this->ThrowIfBadCall(rc, "Prepare select for " + plugin_name);
  utility::datastructure::Table table{plugin_name};

  // Table rows
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    // Table columns
    for (int col = 0; col < sqlite3_data_count(stmt); ++col) {
      std::string col_name = sqlite3_column_name(stmt, col);
      // Handle data columns
      switch (sqlite3_column_type(stmt, col)) {
        case SQLITE_INTEGER: {
          table.AddValue(sqlite3_column_int(stmt, col), col_name);
          break;
        }
        case SQLITE_FLOAT: {
          table.AddValue(sqlite3_column_double(stmt, col), col_name);
          break;
        }
        case SQLITE_TEXT: {
          // Thanks for the unsigned char.
          table.AddValue(std::string(reinterpret_cast<const char *>(
                             sqlite3_column_text(stmt, col))),
                         col_name);
          break;
        }
        case SQLITE_NULL:
        case SQLITE_BLOB:
          throw std::runtime_error("Not supported sql type.");
      }
    }
  }
  return table;
}

void persistenceservice::sqlite::SqliteAdapter::ThrowIfBadCall(
    int rc, const std::string &action, const std::string &execution_info,
    int expected_result) const {
  if (rc != expected_result) {
    if (sqlite3_get_autocommit(this->db_) == 0) {
      sqlite3_exec(this->db_, "ROLLBACK TRANSACTION", nullptr, nullptr,
                   nullptr);
    }
    throw std::runtime_error("Action [" + action +
                             "] failed with return code " + std::to_string(rc) +
                             "! " + execution_info +
                             " Error: " + sqlite3_errmsg(this->db_));
  }
}

void persistenceservice::sqlite::SqliteAdapter::ThrowIfBadCall(
    int rc, const std::string &action, const char *execution_info,
    int expected_result) const {
  if (execution_info) {
    this->ThrowIfBadCall(rc, action, std::string(execution_info),
                         expected_result);
  } else {
    this->ThrowIfBadCall(rc, action, "", expected_result);
  }
}

void persistenceservice::sqlite::SqliteAdapter::PrepareTable(
    const imonitorplugin::PluginData &data) {
  char *err_msg;
  // Check if a table creation is required on first plugin contact
  if (std::find(this->known_plugins_.begin(), this->known_plugins_.end(),
                data.plugin_name()) == this->known_plugins_.end()) {
    auto query =
        persistenceservice::sqlite::QueryHelper::BuildCreateTableQuery(data);
    int rc = sqlite3_exec(this->db_, query.c_str(), QueryCallback, nullptr,
                          &err_msg);
    this->ThrowIfBadCall(rc, "Create table " + data.plugin_name(), err_msg);
    this->known_plugins_.push_back(data.plugin_name());
  }
}

void persistenceservice::sqlite::SqliteAdapter::DoStore(
    const imonitorplugin::PluginData &data) {
  char *err_msg;
  auto query = persistenceservice::sqlite::QueryHelper::BuildInsertQuery(data);
  int rc =
      sqlite3_exec(this->db_, query.c_str(), QueryCallback, nullptr, &err_msg);
  this->ThrowIfBadCall(rc, "Insert into table " + data.plugin_name(), err_msg);
}
