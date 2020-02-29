#ifndef MEASUREMENTPLUGINBASE_H
#define MEASUREMENTPLUGINBASE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "imonitoringplugin/constants.hpp"
#include "imonitoringplugin/imonitorplugin.hpp"
#include "imonitoringplugin/inputfilecontent.hpp"
#include "imonitoringplugin/ipluginconfigselector.hpp"
#include "settingsprovider/isettingsprovider.hpp"

namespace monitoringpluginbase {
/*
 * Macros to define the default function factories.
 * Pass the plugin type as argument.
 * Note that these macros may not be placed inside a class!
 *
 * If you link the plugins instead of load them dynamically, add the
 * LINKED_PLUGIN definition to skip the factory function creation
 */

#ifndef LINKED_PLUGIN
#define CREATE_DETAULT_CONSTRUCTOR_FACTORY(type) \
  extern "C" ::imonitorplugin::IMonitorPlugin*   \
  MONITORING_PLUGIN_CONSTRUCTOR() {              \
    return new type{};                           \
  }
#define CREATE_DEFAULT_DESTRUCTOR_FACTORY(type) \
  extern "C" void MONITORING_PLUGIN_DESTRUCTOR(type* p) { delete p; }
#else
#define CREATE_DETAULT_CONSTRUCTOR_FACTORY(type)
#define CREATE_DEFAULT_DESTRUCTOR_FACTORY(type)
#endif

///
/// \brief The base class of each monitoring plugin.
/// \details Provides basic functionality for plugins. Settings are loaded from
/// files if they are placed next to the plugin library and the file name
/// matches the plugin name + .conf
///
class MonitorPluginBase : virtual public imonitorplugin::IMonitorPlugin {
 public:
  ///
  /// \brief Create new instance
  /// \param name The name of the plugin
  ///
  explicit MonitorPluginBase(std::string name_);

  ///
  /// \copydoc imonitorplugin::IMonitorPlugin::name()
  ///
  [[nodiscard]] std::string name() const override;

  ///
  /// \copydoc imonitorplugin::IMonitorPlugin::input_files()
  ///
  [[nodiscard]] std::vector<std::string> input_files() const override;

  ///
  /// \copydoc imonitorplugin::IMonitorPlugin::AcquireData()
  /// \details Plugin may override this method in order to implement own logik.
  /// Plugins that provide more than one line of data should use this method.
  ///
  virtual std::vector<imonitorplugin::PluginData> AcquireData(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>&&
          input_file) override;

  ///
  /// \copydoc imonitorplugin::IMonitorPlugin::DoSanityCheck()
  ///
  [[nodiscard]] virtual std::vector<std::string> DoSanityCheck() const override;

  ///
  /// \copydoc imonitorplugin::IMonitorPlugin::Configure()
  ///
  void Configure() const override;

 protected:
  ///
  /// \brief This is the easy version of AcquireData and just suffice most
  /// plugins.
  /// \details Plugins will still have to provide an (empty) implementation of
  /// this method, even if AcquireData is used
  /// \param input_file If requested,
  /// the content of a file (2 snapshots) \return A vector of key value pairs
  ///
  virtual imonitorplugin::PluginData::data_vector AcquireDataInternal(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>&&
          input_file) = 0;

  ///
  /// \copydoc imonitorplugin::IMonitorPlugin::GetConfigSelectors()
  /// \note Override this empty implementation to actually expose some selectors
  ///
  virtual std::vector<imonitorplugin::IPluginConfigSelector>
  GetConfigSelectors() const override;
  ///
  /// \brief Raise a plugin exception
  /// \param msg The message to raise
  ///
  void ThrowPluginException [[noreturn]] (const std::string& msg) const;

  ///
  /// \brief Register a file to be read by the plugin framework
  /// \details The content of the file will be passed to AcquireData
  /// \param file The file to register
  ///
  void RegisterFileToRead(const std::string& file);

  ///
  /// \brief Create a unix time stamp
  /// \return The current time in unix time
  ///
  [[nodiscard]] std::int64_t MakeTimestamp() const;

  std::unique_ptr<settingsprovider::ISettingsProvider> settings_;

 private:
  std::string name_;
  std::vector<std::string> input_files_;
};
}  // namespace monitoringpluginbase
#endif  // MEASUREMENTPLUGINBASE_H
