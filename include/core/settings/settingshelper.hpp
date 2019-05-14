#ifndef SETTINGSHELPER_HPP
#define SETTINGSHELPER_HPP

/*
 * Build the settings factory using the settings defined in
 * settingsidentifier.hpp
 * Make sure to actually register them here.
 */

#include <memory>
#include "settingsprovider/settingsfactory.hpp"

namespace core {
namespace settings {

///
/// \brief Register the available settings.
/// \return A new settings factory
///
std::unique_ptr<settingsprovider::SettingsFactory> RegisterFactory();

///
/// \brief Get the application settings
/// \return A new settings provider
///
std::unique_ptr<settingsprovider::ISettingsProvider> GetApplicationSettings();

}  // namespace core
}  // namespace moser2

#endif  // SETTINGSHELPER_HPP
