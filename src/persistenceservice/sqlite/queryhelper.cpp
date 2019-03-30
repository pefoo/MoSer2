#include "persistenceservice/sqlite/queryhelper.hpp"
#include <string>

std::string persistenceservice::sqlite::QueryHelper::BuildCreateTableQuery(
    const imonitorplugin::PluginData& data) {
  std::string query = "create table if not exists " + data.plugin_name() + "(";
  for (const auto& d : data.data()) {
    query += d.first + " " + GetSqliteType(d.second.type()) + ",";
  }
  query += "timestamp INTEGER)";
  return query;
}

std::string persistenceservice::sqlite::QueryHelper::BuildInsertQuery(
    const imonitorplugin::PluginData& data) {
  std::string query = "insert into " + data.plugin_name() + " (";
  for (const auto& d : data.data()) {
    query += d.first + ",";
  }
  query += "timestamp) values (";
  for (auto& d : data.data()) {
    query += GetEscapedValueAsString(d.second) + ",";
  }
  query += std::to_string(data.timestamp()) + ")";
  return query;
}

std::string persistenceservice::sqlite::QueryHelper::BuildSelectQuery(
    const std::string& plugin_name, int64_t min_timestamp) {
  std::string query = "select * from " + plugin_name;
  if (min_timestamp != 0) {
    query += " where timestamp > " + std::to_string(min_timestamp);
  }
  return query;
}

std::string persistenceservice::sqlite::QueryHelper::GetSqliteType(
    const std::type_info& type) {
  // use operator== on some types. Cpplint gets confused about the comparison
  // and reports old style cast errors...
  if (type.operator==(typeid(int)) || type.operator==(typeid(int64_t)) ||
      type.operator==(typeid(bool))) {
    return "INTEGER";
  } else if (type.operator==(typeid(float)) ||
             type.operator==(typeid(double))) {
    return "REAL";
  } else if (type == typeid(std::string)) {
    return "TEXT";
  }

  throw std::runtime_error("Type not supported.");
}

std::string persistenceservice::sqlite::QueryHelper::GetEscapedValueAsString(
    utility::datastructure::Any& value) {
  if (value.type() == typeid(int)) {
    return std::to_string(value.get<int>());
  } else if (value.type() == typeid(float)) {
    return std::to_string(value.get<float>());
  } else if (value.type() == typeid(double)) {
    return std::to_string(value.get<double>());
  } else if (value.type() == typeid(int64_t)) {
    return std::to_string(value.get<int64_t>());
  } else if (value.type() == typeid(std::string)) {
    return "'" + value.get<std::string>() + "'";
  }
  throw std::runtime_error("Type not supported");
}
