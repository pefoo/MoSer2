#ifndef IDATAADAPTER_H
#define IDATAADAPTER_H

#include "imonitoringplugin/plugindata.hpp"
namespace persistenceservice {
///
/// \brief The data adapter interface
/// \details Provides means to store and load data
///
class IDataAdapter {
 public:
  ///
  /// \brief ~IDataAdapter
  ///
  virtual ~IDataAdapter() {}

  ///
  /// \brief Store some data
  /// \param data The data to store
  /// \return True, if storing succeeded
  ///
  virtual void Store(const imonitorplugin::PluginData& data) = 0;

  ///
  /// \brief Load data from the adapter
  /// \param plugin_name The name of the plugin whose data to load
  /// \param min_age The minimum time stamp (unix time)
  /// \return A vector with matching entries
  ///
  virtual std::vector<imonitorplugin::PluginData> Load(
      const std::string& plugin_name, int64_t min_age = 0) = 0;
};

}  // namespace persistenceservice

#endif  // IDATAADAPTER_H
