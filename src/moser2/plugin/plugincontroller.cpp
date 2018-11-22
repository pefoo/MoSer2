#include "plugincontroller.hpp"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "pluginfacade.hpp"
#include "utility/threading/callbacktimer.hpp"
namespace moser2 {
namespace plugin {
PluginController::PluginController()
    : plugin_manager(std::make_unique<MonitoringPluginManager>()),
      execute(false) {}

void PluginController::LoadPlugin(const std::string &path,
                                  const std::string &plugin_name) {
  try {
    this->plugins.push_back(
        this->plugin_manager->LoadPlugin(path, plugin_name));
    LOG(INFO) << "Loaded plugin " << plugin_name << " from " << path;
  } catch (const std::exception &e) {
    LOG(ERROR) << "Failed to load the plugin " << plugin_name << " located at "
               << path << ". " << e.what();
  }
}

void PluginController::LoadPlugins(const std::string &path) {
  // TODO implement LoadPlugins
  throw std::runtime_error("Not implemented");
}

void PluginController::RunPlugins(const int interval_ms) {
  if (this->plugins_running()) {
    this->StopPlugins();
  }

  this->execute.store(true, std::memory_order_release);
  this->plugin_thread = std::thread([=]() {
    utility::threading::CallbackTimer cb_timer{};
    std::mutex cond_lock;
    std::condition_variable cv;

    cb_timer.Start(interval_ms, [&cv]() { cv.notify_one(); });

    while (this->execute.load(std::memory_order_acquire)) {
      std::unique_lock<std::mutex> lk(cond_lock);
      // TODO timeout handling please
      cv.wait(lk);
      std::for_each(std::begin(this->plugins), std::end(this->plugins),
                    [](MonitoringPluginManager::plugin_t *&plug) {
                      // TODO Do something with the data
                      LOG(DEBUG) << plug->Instance()->AcquireData().ToString();
                    });
    }
    cb_timer.Stop();
  });
}

void PluginController::StopPlugins() {
  this->execute.store(false, std::memory_order_release);
  if (this->plugin_thread.joinable()) {
    this->plugin_thread.join();
  }
}

bool PluginController::plugins_running() {
  return this->execute.load(std::memory_order_acquire) &&
         this->plugin_thread.joinable();
}
}  // namespace plugin
}  // namespace moser2
