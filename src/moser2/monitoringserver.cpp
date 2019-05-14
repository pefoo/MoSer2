#include "monitoringserver.hpp"
#include <memory>
#include <utility>
#include "constants/settings/settingsidentifier.hpp"
#include "plugin/plugincontroller.hpp"
#include "pluginmanager/include/plugin_manager.hpp"

moser2::MonitoringServer::MonitoringServer(
    std::shared_ptr<settingsprovider::ISettingsProvider> settings)
    : settings_(std::move(settings)), is_running_(false) {
  this->plugin_controller_ = std::make_unique<plugin::PluginController>();
  this->plugin_controller_->LoadPlugins(
      this->settings_->GetValue(constants::settings::PluginBasePath()),
      this->settings_->GetValue(constants::settings::PluginFilter()));

  // TODO remove this sample code once the persistence runner exists
  //  auto settings =
  //      std::make_unique<persistenceservice::sqlite::SqliteSettings>("foobar.db");
  //  auto adapter_factory =
  //      new persistenceservice::AdapterFactory(std::move(settings));
  //  auto adapter = adapter_factory->CreateAdapter();
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

  this->plugin_controller_->RunPlugins(std::stoi(
      this->settings_->GetValue(constants::settings::MeasurementDelay())));
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
