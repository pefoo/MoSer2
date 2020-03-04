#include "plugincontroller.hpp"
#include <filesystem>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include "easyloggingpp-9.96.5/easylogging++.h"
#include "imonitoringplugin/constants.hpp"
#include "imonitoringplugin/pluginexception.hpp"
#include "pluginfacade.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"
#include "utility/helper/stringhelper.hpp"
#include "utility/threading/callbacktimer.hpp"

namespace moser2 {
namespace plugin {
PluginController::PluginController()
    : plugin_manager_(std::make_unique<MonitoringPluginManager>()),
      timer_(new utility::threading::CallbackTimer{}),
      inputfile_provider_(std::make_unique<InputFileProvider>()) {}

PluginController::~PluginController() { this->plugin_manager_->DestroyAll(); }

void PluginController::LoadPlugin(const std::string &path) {
  auto abs_path = utility::filesystem::MakeAbsolutePathFromExecutable(path);
  try {
    auto plug = this->plugin_manager_->LoadPlugin(
        abs_path, imonitoringplugin::kMonitoringPluginConstructor,
        imonitoringplugin::kMonitoringPluginDestructor);
    plug->Instance()->Init();
    auto input_files = plug->Instance()->input_files();
    if (!input_files.empty()) {
      this->inputfile_provider_->RegisterPluginFiles(plug->Instance()->name(),
                                                     input_files);
    }
    auto msg = plug->Instance()->DoSanityCheck();
    if (!msg.empty()) {
      std::stringstream log;
      log << "Plugin " << plug->Instance()->name()
          << " reporting errors: " << std::endl;
      for (const auto &m : msg) {
        log << m << std::endl;
      }
      LOG(WARNING) << log.str();
      this->plugin_manager_->DestroyPlugin(plug);
    } else {
      this->plugins_.push_back(plug);

      LOG(INFO) << "Loaded plugin " << this->plugins_.back()->Instance()->name()
                << " from " << abs_path;
    }
  } catch (const std::exception &e) {
    LOG(ERROR) << "Failed to load the plugin from " << abs_path << ". "
               << e.what();
  }
}
void PluginController::LoadPlugins(const std::string &path,
                                   const std::string &name_filter) {
  auto abs_path = utility::filesystem::MakeAbsolutePathFromExecutable(path);
  LOG(DEBUG) << "Loading plugins from " + abs_path.string()
             << " using the filter " << name_filter;
  for (const auto &file : std::filesystem::directory_iterator(abs_path)) {
    if (name_filter.empty() ||
        utility::helper::StringRgxGrep(file.path().filename().string(),
                                       name_filter)) {
      this->LoadPlugin(std::filesystem::canonical(file));
    }
  }
}

void PluginController::RunPlugins(const int interval_ms) {
  if (this->plugins_running()) {
    this->StopPlugins();
  }

  // the input file provider waits a second between consecutive reads
  auto adjusted_interval = interval_ms - 1000;
  this->timer_->Start(adjusted_interval, [&]() {
    this->inputfile_provider_->UpdateFiles();
    std::for_each(
        std::begin(this->plugins_), std::end(this->plugins_),
        [&](MonitoringPluginManager::PluginWrapper *&plug) {
          std::vector<imonitorplugin::PluginData> data;
          try {
            data = plug->Instance()->AcquireData(
                this->inputfile_provider_->GetFiles(plug->Instance()->name()));
          } catch (const imonitorplugin::PluginException &pe) {
            LOG(ERROR) << pe.what();
          }
          for (auto &d : data) {
            plugin::PluginFacade::Instance().Put(d);
          }
        });
  });
}

void PluginController::StopPlugins() { this->timer_->Stop(); }

bool PluginController::plugins_running() { return this->timer_->is_running(); }
}  // namespace plugin
}  // namespace moser2
