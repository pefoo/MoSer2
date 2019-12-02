#ifndef IDATAADAPTER_H
#define IDATAADAPTER_H
// LCOV_EXCL_START

#include <string>
#include <vector>
#include "imonitoringplugin/plugindata.hpp"
#include "utility/datastructure/table.hpp"

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
  ///
  virtual void Store(const imonitorplugin::PluginData& data) = 0;

  ///
  /// \brief Store some data
  /// \param begin The data beginning
  /// \param end The data end
  ///
  virtual void Store(
      const std::vector<imonitorplugin::PluginData>::iterator& begin,
      const std::vector<imonitorplugin::PluginData>::iterator& end) = 0;

  ///
  /// \brief Load data from the adapter
  /// \param plugin_name The name of the plugin whose data to load
  /// \param min_age The minimum time stamp (unix time)
  /// \return A vector with matching entries
  ///
  virtual std::vector<imonitorplugin::PluginData> Load(
      const std::string& plugin_name, int64_t min_age = 0) = 0;

  ///
  /// \brief Load data from the adapter using the table structure
  /// \param plugin_name The name of the plugin whose data to laod
  /// \param min_age The minimum time stamp (unix time)
  /// \return A table with the laoded data
  ///
  virtual utility::datastructure::Table LoadTable(
      const std::string& plugin_name, int64_t min_age = 0) = 0;
};

}  // namespace persistenceservice

// LCOV_EXCL_STOP
#endif  // IDATAADAPTER_H
