#include "monitoringserver.hpp"
#include "pluginmanager/include/plugin_manager.hpp"

moser2::MonitoringServer::MonitoringServer() {
  this->pluginManager = std::make_unique<MeasurementPluginManager>();

  // TODO Remove this test code
  auto plugin =
      this->pluginManager->LoadPlugin("./libcpuplugin.so", "cpuplugin");
  auto name = plugin->Instance()->GetName();
}

moser2::MonitoringServer::~MonitoringServer() = default;
