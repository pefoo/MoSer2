#include "settingshelper.hpp"
#include <memory>
#include "constants/settings/settingsidentifier.hpp"
#include "settingsprovider/settingsfactory.hpp"

namespace moser2 {
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
}  // namespace settings
}  // namespace moser2
