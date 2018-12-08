#include "settingsprovider/settingidentifier.hpp"
#include <string>
#include <utility>
#include "settingsprovider/isettingidentifier.hpp"

namespace settingsprovider {
SettingIdentifier::SettingIdentifier(std::string key, std::string section,
                                     Type type)
    : key_(std::move(key)), section_(std::move(section)), type_(type) {}

SettingIdentifier::~SettingIdentifier() {}

std::string SettingIdentifier::key() const { return this->key_; }

std::string SettingIdentifier::section() const { return this->section_; }

ISettingIdentifier::Type SettingIdentifier::type() const { return this->type_; }

}  // namespace settingsprovider
