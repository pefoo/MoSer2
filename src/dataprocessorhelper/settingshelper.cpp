#include "dataprocessorhelper/settingshelper.hpp"
#include <filesystem>
#include <string>
#include "settingsprovider/isettingsprovider.hpp"
#include "settingsprovider/settingsfactory.hpp"

std::unique_ptr<settingsprovider::ISettingsProvider>
dataprocessorhelper::GetPluginSettings(const std::string& plugin_name) {
  if (std::filesystem::exists(plugin_name + ".conf")) {
    settingsprovider::SettingsFactory factory{};
    std::vector<std::string> msg;
    return factory.ReadFromFile(plugin_name + ".conf", &msg);
  }
  return nullptr;
}
