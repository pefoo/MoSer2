#include "settingsprovider/settingnotfoundexception.hpp"

settingsprovider::SettingNotFoundException::SettingNotFoundException(
    const std::string &key, const std::string &section)
    : std::runtime_error("The key: " + key +
                         " was not found for section: " + section) {}
