#include "plugincontroller.hpp"
#include <memory>
#include <string>
#include "easyloggingpp-9.96.5/src/easylogging++.h"

namespace moser2 {
namespace plugin {
PluginController::PluginController() {
  this->plugin_manager = std::make_unique<MonitoringPluginManager>();
}

void PluginController::LoadPlugin(const std::string &path,
                                  const std::string &plugin_name) {
  try {
    this->plugins.push_back(
        this->plugin_manager->LoadPlugin(path, plugin_name));
  } catch (const std::exception &e) {
    LOG(ERROR) << "Failed to load the plugin " << plugin_name << " located at "
               << path << ". " << e.what();
  }
}

void PluginController::LoadPlugins(const std::string &path) {
  // TODO implement LoadPlugins
  throw std::runtime_error("Not implemented");
}
}  // namespace plugin
}  // namespace moser2
