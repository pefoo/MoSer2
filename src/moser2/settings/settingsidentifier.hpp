#ifndef SETTINGSIDENTIFIER_HPP
#define SETTINGSIDENTIFIER_HPP

/*
 * Setting identifier are placed here.
 * To actually use them, register them in settingshelper.cpp
 */

#include <vector>
#include "settingsprovider/settingidentifier.hpp"

namespace moser2 {
namespace settings {
// Because I am lazy...
#define MAKESETTING(name, key, section)                                \
  inline settingsprovider::SettingIdentifier const& name() {           \
    static settingsprovider::SettingIdentifier name_ = {key, section}; \
    return name_;                                                      \
  }

MAKESETTING(MeasurementDelay, "MeasurementDelay", "Core")
MAKESETTING(SqliteDatabaseFile, "DatabaseFile", "SqliteAdapter")
}  // namespace settings
}  // namespace moser2

#endif  // SETTINGSIDENTIFIER_HPP
