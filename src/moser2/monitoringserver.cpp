#include "monitoringserver.hpp"
#include <memory>
#include <utility>
#include "persistenceservice/adapterfactory.hpp"
#include "persistenceservice/sqlite/sqlitesettings.hpp"
#include "plugin/plugincontroller.hpp"
#include "pluginmanager/include/plugin_manager.hpp"

moser2::MonitoringServer::MonitoringServer() {
  this->plugin_controller_ = std::make_unique<plugin::PluginController>();
  // TODO remove this test implementation
  this->plugin_controller_->LoadPlugin("./libcpuplugin.so");

  // TODO remove this sample code :P
  auto settings =
      std::make_unique<persistenceservice::sqlite::SqliteSettings>("foobar.db");
  auto adapter_factory =
      new persistenceservice::AdapterFactory(std::move(settings));
  auto adapter = adapter_factory->CreateAdapter();
}

moser2::MonitoringServer::~MonitoringServer() {
  if (this->plugin_controller_->plugins_running()) {
    this->plugin_controller_->StopPlugins();
  }
}

void moser2::MonitoringServer::Run() {
  if (this->is_running()) {
    return;
  }

  // TODO add measurement frequency to the config file and pass the value to
  // RunPlugins()
  this->plugin_controller_->RunPlugins();
  this->is_running_ = true;
}

void moser2::MonitoringServer::Stop() {
  if (!this->is_running()) {
    return;
  }

  this->plugin_controller_->StopPlugins();
  this->is_running_ = false;
}

bool moser2::MonitoringServer::is_running() const { return this->is_running_; }
