#include "core/settings/settingshelper.hpp"
#include <memory>
#include "constants/constants.hpp"
#include "constants/settings/settingsidentifier.hpp"
#include "core/appsetup.hpp"
#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "settingsprovider/settingsfactory.hpp"

namespace core {
namespace settings {

std::unique_ptr<settingsprovider::SettingsFactory> RegisterFactory() {
  auto factory = std::make_unique<settingsprovider::SettingsFactory>();

  // TODO some default values??
  factory->RegisterSetting(constants::settings::MeasurementDelay(), "",
                           settingsprovider::TypeVerifier<int>::VerifierFunc);
  factory->RegisterSetting(constants::settings::PluginBasePath());
  factory->RegisterSetting(constants::settings::PluginFilter());
  factory->RegisterSetting(constants::settings::SqliteDatabaseFile());

  return factory;
}

std::unique_ptr<settingsprovider::ISettingsProvider> GetApplicationSettings() {
  auto settings_factory = core::settings::RegisterFactory();
  std::vector<std::string> errors;
  auto config_file = core::GetApplicationConfigFile();
  LOG(DEBUG) << "Reading settings from " << config_file;
  auto settings = settings_factory->ReadFromFile(config_file, &errors);

  if (!errors.empty()) {
    for (const auto& e : errors) {
      LOG(ERROR) << e;
    }
    throw std::runtime_error("Detected invalid configuration. See " +
                             std::string(constants::logger_conf) +
                             " for more information.");
  }
  return settings;
}

}  // namespace settings
}  // namespace core
