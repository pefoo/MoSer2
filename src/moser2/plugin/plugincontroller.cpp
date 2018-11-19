#include "plugincontroller.hpp"
#include <memory>
#include <string>

namespace moser2 {
namespace plugin {
PluginController::PluginController() {
  this->pluginManager = std::make_unique<MonitoringPluginManager>();
}

void PluginController::LoadPlugin(const std::string &path,
                                  const std::string &plugin_name) {
  // TODO Remove this test code
  auto plugin =
      this->pluginManager->LoadPlugin("./libcpuplugin.so", "cpuplugin");
  auto name = plugin->Instance()->GetName();
}

void PluginController::LoadPlugins(const std::string &path) {
  // TODO implement LoadPlugins
  throw std::runtime_error("Not implemented");
}
}  // namespace plugin
}  // namespace moser2
