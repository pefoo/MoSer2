#include "settingsprovider/settingsfactory.hpp"
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "settingsprovider/settingsprovider.hpp"
#include "settingsprovider/settingverifier.hpp"

void settingsprovider::SettingsFactory::RegisterSetting(
    const std::string &key, const std::string &section,
    const std::string &default_value, const Verifier &verifier) {
  this->settings_.emplace_back(key, section, default_value, verifier);
}

void settingsprovider::SettingsFactory::RegisterSetting(
    const settingsprovider::SettingIdentifier &identifier,
    const std::string &default_value, const Verifier &verifier) {
  this->RegisterSetting(identifier.key(), identifier.section(), default_value,
                        verifier);
}

std::unique_ptr<settingsprovider::ISettingsProvider>
settingsprovider::SettingsFactory::ReadFromFile(const std::string &file,
                                                std::vector<std::string> *msg) {
  auto settings = std::make_unique<settingsprovider::SettingsProvider>();
  for (const Setting &s : this->settings_) {
    settings->RegisterSetting(s.key_, s.section_, s.value_, s.verifier_);
  }
  settings->ReadFromFile(file, msg);
  return std::move(settings);
}

void settingsprovider::SettingsFactory::WriteToFile(const std::string &file) {
  settingsprovider::SettingsProvider settings{};
  for (const Setting &s : this->settings_) {
    settings.RegisterSetting(s.key_, s.section_, s.value_, s.verifier_);
  }
  settings.WriteToFile(file);
}
