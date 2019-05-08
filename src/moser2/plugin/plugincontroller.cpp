#include "plugincontroller.hpp"
#include <condition_variable>
#include <filesystem>
#include <memory>
#include <mutex>
#include <regex>
#include <string>
#include <utility>
#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "imonitoringplugin/pluginexception.hpp"
#include "pluginfacade.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"
#include "utility/threading/callbacktimer.hpp"

namespace moser2 {
namespace plugin {
PluginController::PluginController()
    : plugin_manager_(std::make_unique<MonitoringPluginManager>()),
      timer_(new utility::threading::CallbackTimer{}) {}

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
  LOG(DEBUG) << "Loading plugins from " + path << " using the filter "
             << name_filter;
  std::regex rgx{name_filter};
  for (const auto &file : std::filesystem::directory_iterator(path)) {
    if (name_filter.empty() ||
        std::regex_match(file.path().filename().string(), rgx)) {
      this->LoadPlugin(std::filesystem::canonical(file));
    }
  }
}

void PluginController::RunPlugins(const int interval_ms) {
  if (this->plugins_running()) {
    this->StopPlugins();
  }

  this->timer_->Start(interval_ms, [&]() {
    std::for_each(std::begin(this->plugins_), std::end(this->plugins_),
                  [](MonitoringPluginManager::plugin_t *&plug) {
                    // Bug notes:
                    // Workaround: this actually copies the wrapped data of the
                    // any class
                    // Works only for data type float though...
                    // The copy constructor of the the Any type should actually
                    // deep copy the wrapped data
                    imonitorplugin::PluginData::data_vector c;
                    imonitorplugin::PluginData orig_data;
                    try {
                      orig_data = plug->Instance()->AcquireData();
                    } catch (const imonitorplugin::PluginException &pe) {
                      LOG(ERROR) << pe.what();
                    }

                    //          for (auto d : orig_data.data()) {
                    //            auto t = d.second.get<float>();
                    //            c.push_back({d.first,
                    //            utility::datastructure::Any{std::move(t)}});
                    //          }
                    //          imonitorplugin::PluginData
                    //          d{orig_data.plugin_name(),
                    //                                       orig_data.timestamp(),
                    //                                       c};
                    plugin::PluginFacade::Instance().Put(orig_data);
                  });
  });
}

void PluginController::StopPlugins() { this->timer_->Stop(); }

bool PluginController::plugins_running() { return this->timer_->is_running(); }
}  // namespace plugin
}  // namespace moser2
