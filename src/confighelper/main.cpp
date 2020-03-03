#include "constants/settings/settingsidentifier.hpp"
#include "core/appsetup.hpp"
#include "core/settings/settingshelper.hpp"
#include "easyloggingpp-9.96.5/easylogging++.h"
#include "imonitoringplugin/constants.hpp"
#include "imonitoringplugin/imonitorplugin.hpp"
#include "pluginmanager/pluginloader.hpp"
#include "pluginmanager/pluginmanager.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"
#include "utility/helper/stringhelper.hpp"

INITIALIZE_EASYLOGGINGPP

int main() {
  core::ConfigureLogger();
  std::shared_ptr<settingsprovider::ISettingsProvider> settings =
      core::settings::GetApplicationSettings();

  using PluginLoader =
      pluginmanager::PluginManager<imonitorplugin::IMonitorPlugin>;
  std::vector<PluginLoader::PluginWrapper *> plugins_;
  PluginLoader loader{};
  auto base_path = settings->GetValue(constants::settings::PluginBasePath());
  auto name_filter = settings->GetValue(constants::settings::PluginFilter());
  auto abs_path =
      utility::filesystem::MakeAbsolutePathFromExecutable(base_path);

  for (const auto &file : std::filesystem::directory_iterator(abs_path)) {
    if (name_filter.empty() ||
        utility::helper::StringRgxGrep(file.path().filename().string(),
                                       name_filter)) {
      plugins_.push_back(
          loader.LoadPlugin(std::filesystem::canonical(file),
                            imonitoringplugin::kMonitoringPluginConstructor,
                            imonitoringplugin::kMonitoringPluginDestructor));
    }
  }

  for (const auto &plugin : plugins_) {
    plugin->Instance()->Configure();
  }

  loader.DestroyAll();
  return 0;
}
