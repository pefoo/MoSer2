#ifndef SETTINGSIDENTIFIER_HPP
#define SETTINGSIDENTIFIER_HPP

/*
 * Setting identifier are placed here.
 * To actually use them, register them in settingshelper.cpp
 */

#include <vector>
#include "settingsprovider/settingidentifier.hpp"

namespace constants {
namespace settings {
// Because I am lazy...
#define MAKESETTING(name, key, section)                                \
  inline settingsprovider::SettingIdentifier const& name() {           \
    static settingsprovider::SettingIdentifier name_ = {key, section}; \
    return name_;                                                      \
  }

MAKESETTING(MeasurementDelay, "MeasurementDelay", "Core")
MAKESETTING(PluginBasePath, "PluginBasePath", "Core")
MAKESETTING(PluginFilter, "PluginFilter", "Core")

MAKESETTING(SqliteDatabaseFile, "DatabaseFile", "SqliteAdapter")

MAKESETTING(DataAge, "DataAge", "Reporter")
MAKESETTING(ReporTemplate, "ReportTemplate", "Reporter")
MAKESETTING(MailRecipient, "MailRecipient", "Reporter")
MAKESETTING(SmtpServer, "SmtpServer", "Reporter")
MAKESETTING(MailUser, "MailUser", "Reporter")
MAKESETTING(MailPassword, "MailPassword", "Reporter")
MAKESETTING(PostProcessingScript, "PostProcessingScript", "Reporter");
}  // namespace settings
}  // namespace constants

#endif  // SETTINGSIDENTIFIER_HPP
