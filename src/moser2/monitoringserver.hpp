#ifndef MONITORINGSERVER_H
#define MONITORINGSERVER_H

#include <memory>

namespace moser2 {
namespace plugin {
class PluginController;
}  // namespace plugin

///
/// \brief The monitoring server entry point.
///
class MonitoringServer {
 public:
  ///
  /// \brief MonitoringServer
  ///
  MonitoringServer();

  ///
  /// \brief ~MonitoringServer
  ///
  ~MonitoringServer();

 private:
  std::unique_ptr<plugin::PluginController> plugin_controller_;
};
}  // namespace moser2

#endif  // MONITORINGSERVER_H
