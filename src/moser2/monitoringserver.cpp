#include "monitoringserver.hpp"
#include <memory>
#include "plugin/plugincontroller.hpp"
#include "pluginmanager/include/plugin_manager.hpp"

moser2::MonitoringServer::MonitoringServer() {
  this->plugin_controller_ = std::make_unique<plugin::PluginController>();
  // TODO remove this test implementation
  this->plugin_controller_->LoadPlugin("./libcpuplugin.so");
  this->plugin_controller_->RunPlugins();
}

moser2::MonitoringServer::~MonitoringServer() {
  if (this->plugin_controller_->plugins_running()) {
    this->plugin_controller_->StopPlugins();
  }
}
