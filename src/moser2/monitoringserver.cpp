#include "monitoringserver.hpp"
#include <memory>
#include "plugin/plugincontroller.hpp"
#include "pluginmanager/include/plugin_manager.hpp"

moser2::MonitoringServer::MonitoringServer() {
  this->plugin_controller = std::make_unique<plugin::PluginController>();
}

moser2::MonitoringServer::~MonitoringServer() = default;
