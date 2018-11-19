#include "plugincontroller.hpp"

using namespace moser2::plugin;

PluginController::PluginController() {
  this->pluginManager = std::make_unique<MonitoringPluginManager>();
}

void PluginController::LoadPlugin(const std::string &path) {
  // TODO Remove this test code
  auto plugin =
      this->pluginManager->LoadPlugin("./libcpuplugin.so", "cpuplugin");
  auto name = plugin->Instance()->GetName();
}

void PluginController::LoadPlugins(const std::string &path) {
  // TODO implement LoadPlugins
  throw std::runtime_error("Not implemented");
}
