#ifndef MONITORINGSERVER_H
#define MONITORINGSERVER_H

#include <memory>

namespace moser2 {
namespace plugin {
class PluginController;
}  // namespace plugin

///
/// \brief TODO
///
class MonitoringServer {
 public:
  MonitoringServer();
  ~MonitoringServer();

 private:
  std::unique_ptr<plugin::PluginController> plugin_controller;
};
}  // namespace moser2

#endif  // MONITORINGSERVER_H
