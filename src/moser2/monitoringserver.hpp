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

  ///
  /// \brief Run the monitoring server
  ///
  void Run();

  ///
  /// \brief Stop the monitoring server
  ///
  void Stop();

  ///
  /// \brief Get whether the server is running
  /// \return True, if the server is running
  ///
  bool is_running() const;

 private:
  std::unique_ptr<plugin::PluginController> plugin_controller_;
  bool is_running_;
};
}  // namespace moser2

#endif  // MONITORINGSERVER_H
