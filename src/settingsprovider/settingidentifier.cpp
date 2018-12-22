#include "settingsprovider/settingidentifier.hpp"
#include <string>
#include <utility>

namespace settingsprovider {
SettingIdentifier::SettingIdentifier(std::string key, std::string section)
    : key_(std::move(key)), section_(std::move(section)) {}

SettingIdentifier::~SettingIdentifier() = default;

std::string SettingIdentifier::key() const { return this->key_; }

std::string SettingIdentifier::section() const { return this->section_; }

}  // namespace settingsprovider
