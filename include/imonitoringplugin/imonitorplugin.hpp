#ifndef IMONITORPLUGIN_H
#define IMONITORPLUGIN_H

#include <string>
#include "imonitoringplugin/inputfilecontent.hpp"
#include "imonitoringplugin/plugindata.hpp"

namespace imonitorplugin {
///
/// \brief The interface monitoring plugins have to implement
/// \note This interface is implemented by
/// monitoringpluginbase::MonitorPluginBase
///
class IMonitorPlugin {
 public:
  ///
  /// \brief ~IMonitorPlugin
  ///
  virtual ~IMonitorPlugin() {}

  ///
  /// \brief Get the plugin name
  /// \return The name of the plugin
  ///
  virtual std::string name() const = 0;

  ///
  /// \brief Get the requested input file name
  /// \return The file name
  ///
  virtual std::string input_file() const = 0;

  ///
  /// \brief Acquire the data.
  /// \param input_file If requested, the content of a file (2 snapshots)
  /// \return The acquired data.
  ///
  virtual PluginData AcquireData(
      imonitorplugin::InputFileContent&& input_file) const = 0;
};

}  // namespace imonitorplugin
#endif  // IMONITORPLUGIN_H
