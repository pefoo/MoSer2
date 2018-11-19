#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <memory>
#include <string>
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
  /// \param path The path to the file to laod the plugin from (a shared
  /// library)
  ///
  void LoadPlugin(const std::string& path, const std::string& plugin_ame);

  ///
  /// \brief Try to load plugins from all shared libraries in a folder
  /// \param path The path to the folder to the plugings from
  ///
  [[noreturn]] void LoadPlugins(const std::string& path);

 private:
  typedef pluginmanager::PluginManager<imonitorplugin::IMonitorPlugin,
                                       imonitorplugin::create_t,
                                       imonitorplugin::destroy_t>
      MonitoringPluginManager;
  std::unique_ptr<MonitoringPluginManager> pluginManager;
  std::vector<imonitorplugin::IMonitorPlugin> plugins;
};
}  // namespace plugin
}  // namespace moser2
#endif  // PLUGINCONTROLLER_H
