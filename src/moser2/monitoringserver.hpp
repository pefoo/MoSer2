#ifndef MONITORINGSERVER_H
#define MONITORINGSERVER_H

#include <memory>
#include "settingsprovider/isettingsprovider.hpp"

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
  explicit MonitoringServer(
      std::shared_ptr<settingsprovider::ISettingsProvider> settings);

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
  [[nodiscard]] bool is_running() const;

 private:
  std::unique_ptr<plugin::PluginController> plugin_controller_;
  std::shared_ptr<settingsprovider::ISettingsProvider> settings_;
  bool is_running_;
};
}  // namespace moser2

#endif  // MONITORINGSERVER_H
