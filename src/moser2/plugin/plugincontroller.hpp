#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "imonitoringplugin/imonitorplugin.hpp"
#include "pluginmanager/include/plugin_manager.hpp"

namespace moser2 {
namespace plugin {
///
/// \brief The monitoring plugin controller.
///
class PluginController {
 public:
  ///
  /// \brief PluginController
  ///
  PluginController();
  ///
  /// \brief Load a single plugin
  /// \param path The path to the file to load the plugin from (a shared
  /// library)
  ///
  void LoadPlugin(const std::string& path);

  ///
  /// \brief Try to load plugins from all shared libraries in a folder
  /// \param path The path to the folder to the plugins from
  ///
  [[noreturn]] void LoadPlugins(const std::string& path);

  ///
  /// \brief Run the plugins
  /// \param interval_ms The interval to use (default is 10s)
  /// \details Plugins have to be loaded at this point.
  /// The plugins are called every interval_ms milliseconds.
  /// Running the plugins starts 2 new threads. One for the timer, another one
  /// is used for the actual plugin execution.
  ///
  void RunPlugins(const int interval_ms = 10000);

  ///
  /// \brief Stop the plugin execution
  ///
  void StopPlugins();

  ///
  /// \brief Get whether the plugins are running
  /// \return True, if the plugins are running
  ///
  bool plugins_running();

 private:
  typedef pluginmanager::PluginManager<imonitorplugin::IMonitorPlugin,
                                       imonitorplugin::create_t,
                                       imonitorplugin::destroy_t>
      MonitoringPluginManager;
  std::unique_ptr<MonitoringPluginManager> plugin_manager_;
  std::atomic_bool execute_;
  std::thread plugin_thread_;
  std::vector<MonitoringPluginManager::plugin_t*> plugins_;
};
}  // namespace plugin
}  // namespace moser2
#endif  // PLUGINCONTROLLER_H
