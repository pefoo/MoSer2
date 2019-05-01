#ifndef SETTINGSHELPER_HPP
#define SETTINGSHELPER_HPP

/*
 * Build the settings factory using the settings defined in
 * settingsidentifier.hpp
 * Make sure to actually register them here.
 */

#include <memory>
#include "settingsprovider/settingsfactory.hpp"

namespace moser2 {
namespace settings {

///
/// \brief Register the available settings.
/// \return A new settings factory
///
std::unique_ptr<settingsprovider::SettingsFactory> RegisterFactory();

}  // namespace settings
}  // namespace moser2

#endif  // SETTINGSHELPER_HPP
