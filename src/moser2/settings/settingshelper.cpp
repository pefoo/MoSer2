#include "settingshelper.hpp"
#include <memory>
#include "moser2/settings/settingsidentifier.hpp"
#include "settingsprovider/settingsfactory.hpp"

using namespace moser2::settings;

std::unique_ptr<settingsprovider::SettingsFactory>
moser2::settings::RegisterFactory() {
  auto factory = std::make_unique<settingsprovider::SettingsFactory>();

  factory->RegisterSetting(MeasurementDelay(), "",
                           settingsprovider::TypeVerifier<int>::VerifierFunc);
  factory->RegisterSetting(SqliteDatabaseFile());

  return factory;
}
