#ifndef IMONITORPLUGIN_H
#define IMONITORPLUGIN_H
// LCOV_EXCL_START

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "imonitoringplugin/inputfilecontent.hpp"
#include "imonitoringplugin/ipluginconfigselector.hpp"
#include "imonitoringplugin/plugindata.hpp"

namespace imonitorplugin {
///
/// \brief The interface monitoring plugins have to implement
/// \details Plugin construction should not depend on actual plugin
/// configuration, as imonitorplugin::IMonitorPlugin::Configure() requires an
/// instance. The moser2::plugin::PluginController will always run the
/// imonitorplugin::IMonitorPlugin::DoSanityCheck() function the check the
/// provided plugin configuration after actually initializing the plugin using
/// imonitorplugin::IMonitorPlugin::Init()
///
class IMonitorPlugin {
 public:
  ///
  /// \brief ~IMonitorPlugin
  ///
  virtual ~IMonitorPlugin() = default;

  ///
  /// \brief Initialize the plugin
  ///
  virtual void Init() = 0;

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
  virtual void Configure() = 0;

 protected:
  ///
  /// \brief Get the configuration selectors defined by the plugin
  /// \param os The output stream to write to during configuration
  /// \param is The input stream to read from during configuration
  /// \return A vector with configuration selectors
  ///
  [[nodiscard]] virtual std::vector<
      std::shared_ptr<imonitorplugin::IPluginConfigSelector>>
  GetConfigSelectors(std::ostream& os, std::istream& is) const = 0;
};

}  // namespace imonitorplugin
// LCOV_EXCL_STOP
#endif  // IMONITORPLUGIN_H
