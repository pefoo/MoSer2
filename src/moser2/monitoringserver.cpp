#include "monitoringserver.hpp"
#include <memory>
#include "plugin/plugincontroller.hpp"
#include "pluginmanager/include/plugin_manager.hpp"

moser2::MonitoringServer::MonitoringServer() {
  this->plugin_controller = std::make_unique<plugin::PluginController>();
  // TODO remove this test implementation
  this->plugin_controller->LoadPlugin("./libcpuplugin.so", "cpuPlugin");
  this->plugin_controller->RunPlugins();
}

moser2::MonitoringServer::~MonitoringServer() {
  if (this->plugin_controller->plugins_running()) {
    this->plugin_controller->StopPlugins();
  }
}
