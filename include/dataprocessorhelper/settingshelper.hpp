#ifndef DPSETTINGSHELPER_H
#define DPSETTINGSHELPER_H

#include <memory>
#include <string>
#include "settingsprovider/isettingsprovider.hpp"

namespace dataprocessorhelper {

std::unique_ptr<settingsprovider::ISettingsProvider> GetPluginSettings(
    const std::string& plugin_name);

}  // namespace dataprocessorhelper

#endif  // DPSETTINGSHELPER_H
