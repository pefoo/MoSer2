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
  std::string name() const override;

  ///
  /// \copydoc imonitorplugin::IMonitorPlugin::input_files()
  ///
  std::vector<std::string> input_files() const override;

  ///
  /// \copydoc imonitorplugin::IMonitorPlugin::AcquireData()
  ///
  imonitorplugin::PluginData AcquireData(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>&&
          input_file) const override;

  ///
  /// \copydoc imonitorplugin::IMonitorPlugin::DoSanityCheck()
  ///
  virtual std::vector<std::string> DoSanityCheck() const override;

 protected:
  ///
  /// \brief Here the actual work happens. Plugins override this method.
  /// \details Plugins are not responsible for packing the data.
  /// \param input_file If requested, the content of a file (2 snapshots)
  /// \return A vector of key value pairs
  ///
  virtual imonitorplugin::PluginData::data_vector AcquireDataInternal(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>&&
          input_file) const = 0;

  ///
  /// \brief Raise a plugin exception
  /// \param msg The message to raise
  ///
  void ThrowPluginException[[noreturn]](const std::string& msg) const;

  ///
  /// \brief Register a file to be read by the plugin framework
  /// \details The content of the file will be passed to AcquireData
  /// \param file The file to register
  ///
  void RegisterFileToRead(const std::string& file);

  std::unique_ptr<settingsprovider::ISettingsProvider> settings_;

 private:
  std::int64_t MakeTimestamp() const;
  std::string name_;
  std::vector<std::string> input_files_;
};
}  // namespace monitoringpluginbase
#endif  // MEASUREMENTPLUGINBASE_H
