#ifndef ISETTINGSPROVIDER_H
#define ISETTINGSPROVIDER_H

#include <functional>
#include <string>
#include "settingidentifier.hpp"

namespace settingsprovider {
// Ignore interfaces when calculating code coverage
// LCOV_EXCL_START

class ISettingsProvider {
 public:
  ///
  /// \brief ISettingsProvider
  ///
  ISettingsProvider() {}

  ///
  /// \brief ~ISettingsProvider
  ///
  virtual ~ISettingsProvider() {}

  ///
  /// \brief Get the value
  /// \param key The key of the value
  /// \param section The section of the value
  /// \return The requested setting
  ///
  virtual std::string GetValue(const std::string& key,
                               const std::string& section) const = 0;

  ///
  /// \brief Get the value
  /// \param identifier The identifier of the value to get
  /// \return The requested setting
  ///
  virtual std::string GetValue(const SettingIdentifier& identifier) const = 0;
};

// LCOV_EXCL_STOP

}  // namespace settingsprovider

#endif  // ISETTINGSPROVIDER_H
