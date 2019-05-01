#include "settingshelper.hpp"
#include <memory>
#include "moser2/settings/settingsidentifier.hpp"
#include "settingsprovider/settingsfactory.hpp"

namespace moser2 {
namespace settings {

std::unique_ptr<settingsprovider::SettingsFactory> RegisterFactory() {
  auto factory = std::make_unique<settingsprovider::SettingsFactory>();

  // TODO some default values??
  factory->RegisterSetting(MeasurementDelay(), "",
                           settingsprovider::TypeVerifier<int>::VerifierFunc);
  factory->RegisterSetting(PluginBasePath());
  factory->RegisterSetting(PluginFilter());
  factory->RegisterSetting(SqliteDatabaseFile());

  return factory;
}
}  // namespace settings
}  // namespace moser2
