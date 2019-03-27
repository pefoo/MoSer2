#include "persistenceservice/sqlite/queryhelper.hpp"
#include <string>

std::string persistenceservice::sqlite::QueryHelper::BuildCreateTableQuery(
    const imonitorplugin::PluginData& data) {
  std::string query = "create table if not exists " + data.plugin_name() + "(";
  for (const auto& d : data.data()) {
  }
}
