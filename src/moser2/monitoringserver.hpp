#ifndef MONITORINGSERVER_H
#define MONITORINGSERVER_H
#include <memory>
#include "monitorplugin/imonitorplugin.hpp"
#include "pluginmanager/include/plugin_manager.hpp"

namespace moser2 {
///
/// \brief TODO
///
class MonitoringServer {
 public:
  MonitoringServer();
  ~MonitoringServer();

 private:
  typedef pluginmanager::PluginManager<monitorplugin::IMonitorPlugin,
                                       monitorplugin::create_t,
                                       monitorplugin::destroy_t>
      MeasurementPluginManager;
  std::unique_ptr<MeasurementPluginManager> pluginManager;
};
}

#endif  // MONITORINGSERVER_H
