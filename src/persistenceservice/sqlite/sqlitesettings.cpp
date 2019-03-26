#include "persistenceservice/sqlite/sqlitesettings.hpp"
#include <utility>

persistenceservice::sqlite::SqliteSettings::SqliteSettings(
    std::string file_name)
    : persistenceservice::AdapterSettings(
          persistenceservice::AdapterTypes::sqlite),
      file_name_(std::move(file_name)) {}

std::string persistenceservice::sqlite::SqliteSettings::file_name() const {
  return this->file_name_;
}
