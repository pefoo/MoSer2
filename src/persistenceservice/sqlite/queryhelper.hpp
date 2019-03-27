#ifndef QUERYHELPER_H
#define QUERYHELPER_H

#include <string>
#include "imonitoringplugin/plugindata.hpp"

namespace persistenceservice {
namespace sqlite {
///
/// \brief TODO
///
class QueryHelper {
 public:
  static std::string BuildCreateTableQuery(const imonitorplugin::PluginData&);

 private:
  QueryHelper() {}
};
}  // namespace sqlite
}  // namespace persistenceservice

#endif  // QUERYHELPER_H
