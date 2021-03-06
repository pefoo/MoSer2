#ifndef ISETTINGSPROVIDER_H
#define ISETTINGSPROVIDER_H
// LCOV_EXCL_START

#include <functional>
#include <string>
#include <vector>
#include "settingidentifier.hpp"

namespace settingsprovider {
class ISettingsProvider {
 public:
  ///
  /// \brief ISettingsProvider
  ///
  ISettingsProvider() = default;

  ///
  /// \brief ~ISettingsProvider
  ///
  virtual ~ISettingsProvider() = default;

  ///
  /// \brief Get the value
  /// \param key The key of the value
  /// \param section The section of the value
  /// \return The requested setting
  ///
  [[nodiscard]] virtual std::string GetValue(
      const std::string& key, const std::string& section) const = 0;

  ///
  /// \brief Get the value
  /// \param identifier The identifier of the value to get
  /// \return The requested setting
  ///
  [[nodiscard]] virtual std::string GetValue(
      const SettingIdentifier& identifier) const = 0;

  ///
  /// \brief Get a list of all available sections
  /// \return A vector with all section of the configuration file
  ///
  [[nodiscard]] virtual std::vector<std::string> Sections() const = 0;
};

}  // namespace settingsprovider
// LCOV_EXCL_STOP
#endif  // ISETTINGSPROVIDER_H
