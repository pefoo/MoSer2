#ifndef IMONITORPLUGIN_H
#define IMONITORPLUGIN_H
// LCOV_EXCL_START

#include <string>
#include <unordered_map>
#include <vector>
#include "imonitoringplugin/inputfilecontent.hpp"
#include "imonitoringplugin/ipluginconfigselector.hpp"
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
  virtual ~IMonitorPlugin() = default;

  ///
  /// \brief Get the plugin name
  /// \return The name of the plugin
  ///
  [[nodiscard]] virtual std::string name() const = 0;

  ///
  /// \brief Get the requested input files
  /// \return The file names
  ///
  [[nodiscard]] virtual std::vector<std::string> input_files() const = 0;

  ///
  /// \brief Acquire the data.
  /// \param input_file If requested, the content of a file (2 snapshots)
  /// \return The acquired data.
  ///
  virtual std::vector<PluginData> AcquireData(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>&&
          input_file) = 0;

  ///
  /// \brief Check the plugin configuration and basic functionality
  /// \return A vector of messages, empty if everything is ok
  ///
  [[nodiscard]] virtual std::vector<std::string> DoSanityCheck() const = 0;

  ///
  /// \brief Executes the registered configuration selectors and writes the
  /// configuration file for the plugin
  ///
  virtual void Configure() const = 0;

 protected:
  ///
  /// \brief Get the configuration selectors defined by the plugin
  /// \return A vector with configuration selectors
  ///
  [[nodiscard]] virtual std::vector<imonitorplugin::IPluginConfigSelector>
  GetConfigSelectors() const = 0;
};

}  // namespace imonitorplugin
// LCOV_EXCL_STOP
#endif  // IMONITORPLUGIN_H
