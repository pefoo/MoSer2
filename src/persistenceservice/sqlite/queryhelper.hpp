#ifndef QUERYHELPER_H
#define QUERYHELPER_H

#include <map>
#include <string>
#include "imonitoringplugin/plugindata.hpp"
#include "utility/datastructure/Any.hpp"

namespace persistenceservice {
namespace sqlite {
///
/// \brief A helper to create sqlite queries
/// \details Plugin names are used as table names
///
class QueryHelper {
 public:
  ///
  /// \brief Build a create table query
  /// \details Create table if not exists is used
  /// \return A query to build a new table based on a plugin data object
  ///
  static std::string BuildCreateTableQuery(const imonitorplugin::PluginData&);

  ///
  /// \brief Build a insert query
  /// \return A query that inserts the provided data into the right table
  ///
  static std::string BuildInsertQuery(const imonitorplugin::PluginData&);

 private:
  QueryHelper() {}
  static std::string GetSqliteType(const std::type_info& type);
  static std::string GetEscapedValueAsString(utility::datastructure::Any&);
};
}  // namespace sqlite
}  // namespace persistenceservice

#endif  // QUERYHELPER_H
