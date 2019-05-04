#ifndef PERSISTENCERUNNER_H
#define PERSISTENCERUNNER_H

#include <functional>
#include <memory>
#include "imonitoringplugin/plugindata.hpp"
#include "persistenceservice/idataadapter.hpp"
#include "utility/threading/callbacktimer.hpp"

namespace persistenceservice {
///
/// \brief The persistence runner. Takes data from any kind of data source and
/// stores it using any data adapter
/// TODO: Implement destructor to abort the service
class PersistenceRunner {
 public:
  ///
  /// \brief Create a new instance
  /// \param data_adapter The adapter to store the data
  /// \param source A data source
  ///
  PersistenceRunner(std::unique_ptr<IDataAdapter> data_adapter,
                    std::function<bool(imonitorplugin::PluginData*)> source);

  ///
  /// \brief Run the service
  /// \param delay The delay  in [ms] between consecutive stores
  ///
  void Run(int delay);

  ///
  /// \brief Stop the service
  ///
  void Stop();

 private:
  std::unique_ptr<IDataAdapter> data_adapter_;
  std::function<bool(imonitorplugin::PluginData*)> source_func_;
  std::unique_ptr<utility::threading::CallbackTimer> timer_;
};
}  // namespace persistenceservice

#endif  // PERSISTENCERUNNER_H
