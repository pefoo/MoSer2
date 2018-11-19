#ifndef MONITORINGSERVER_H
#define MONITORINGSERVER_H
#include <memory>
#include "imonitoringplugin/imonitorplugin.hpp"
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
  typedef pluginmanager::PluginManager<imonitorplugin::IMonitorPlugin,
                                       imonitorplugin::create_t,
                                       imonitorplugin::destroy_t>
      MeasurementPluginManager;
  std::unique_ptr<MeasurementPluginManager> pluginManager;
};
}

#endif  // MONITORINGSERVER_H
