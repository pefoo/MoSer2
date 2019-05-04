#ifndef PERSISTENCERUNNER_H
#define PERSISTENCERUNNER_H

#include <functional>
#include <memory>
#include "imonitoringplugin/plugindata.hpp"
#include "persistenceservice/idataadapter.hpp"

namespace persistenceservice {
///
/// \brief TODO
///
class PersistenceRunner {
 public:
  PersistenceRunner(std::unique_ptr<IDataAdapter> data_adapter,
                    std::function<bool(imonitorplugin::PluginData*)> source);

 private:
  std::unique_ptr<IDataAdapter> data_adapter_;
  std::function<bool(imonitorplugin::PluginData*)> source_func_;
};
}  // namespace persistenceservice

#endif  // PERSISTENCERUNNER_H
