#include "plugincontroller.hpp"
#include <condition_variable>
#include <memory>
#include <mutex>
#include <regex>
#include <string>
#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "pluginfacade.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"
#include "utility/threading/callbacktimer.hpp"

namespace moser2 {
namespace plugin {
PluginController::PluginController()
    : plugin_manager_(std::make_unique<MonitoringPluginManager>()),
      execute_(false) {}

void PluginController::LoadPlugin(const std::string &path) {
  try {
    this->plugins_.push_back(this->plugin_manager_->LoadPlugin(path));
    LOG(INFO) << "Loaded plugin " << this->plugins_.back()->Instance()->name()
              << " from " << path;
  } catch (const std::exception &e) {
    LOG(ERROR) << "Failed to load the plugin "
               << this->plugins_.back()->Instance()->name() << " located at "
               << path << ". " << e.what();
  }
}
void PluginController::LoadPlugins(const std::string &path,
                                   const std::string &name_filter) {
  auto files = utility::filesystem::ListFiles(path);
  if (files.size() == 0) {
    LOG(WARNING) << "No plugins found in " << path;
    return;
  }

  std::regex rgx{name_filter};
  for (const auto &file : files) {
    if (name_filter == "" || std::regex_match(file, rgx)) {
      this->LoadPlugin(
          utility::filesystem::MakeAbsolutePath(path + "/" + file));
    }
  }
}

void PluginController::RunPlugins(const int interval_ms) {
  if (this->plugins_running()) {
    this->StopPlugins();
  }

  this->execute_.store(true, std::memory_order_release);
  this->plugin_thread_ = std::thread([=]() {
    utility::threading::CallbackTimer cb_timer{};
    std::mutex cond_lock;
    std::condition_variable cv;

    cb_timer.Start(interval_ms, [&cv]() { cv.notify_one(); });

    while (this->execute_.load(std::memory_order_acquire)) {
      std::unique_lock<std::mutex> lk(cond_lock);
      // TODO timeout handling please
      cv.wait(lk);
      if (!this->execute_.load(std::memory_order_acquire)) break;
      std::for_each(
          std::begin(this->plugins_), std::end(this->plugins_),
          [](MonitoringPluginManager::plugin_t *&plug) {
            // Bug notes:
            // Workaround: this actually copies the wrapped data of the
            // any class
            // Works only for data type float though...
            // The copy constructor of the the Any type should actually deep
            // copy the wrapped data
            imonitorplugin::PluginData::data_vector c;
            auto orig_data = plug->Instance()->AcquireData();

            for (auto d : orig_data.data()) {
              auto t = d.second.get<float>();
              c.push_back({d.first, utility::datastructure::Any{std::move(t)}});
            }
            imonitorplugin::PluginData d{orig_data.plugin_name(),
                                         orig_data.timestamp(), c};
            plugin::PluginFacade::Instance().Put(d);
          });
    }
    cb_timer.Stop();
  });
}

void PluginController::StopPlugins() {
  this->execute_.store(false, std::memory_order_release);
  if (this->plugin_thread_.joinable()) {
    this->plugin_thread_.join();
  }
}

bool PluginController::plugins_running() {
  return this->execute_.load(std::memory_order_acquire) &&
         this->plugin_thread_.joinable();
}
}  // namespace plugin
}  // namespace moser2
