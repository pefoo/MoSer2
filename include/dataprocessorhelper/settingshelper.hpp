#ifndef DPSETTINGSHELPER_H
#define DPSETTINGSHELPER_H

#include "settingsprovider/isettingsprovider.hpp"
#include <memory>

namespace dataprocessorhelper {

std::unique_ptr<settingsprovider::ISettingsProvider> GetPluginSettings(const std::string& plugin_name);

} // namespace dataprocessorhelper

#endif //DPSETTINGSHELPER_H
