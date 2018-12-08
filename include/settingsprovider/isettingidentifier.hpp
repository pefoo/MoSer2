#ifndef ISETTINGIDENTIFIER_H
#define ISETTINGIDENTIFIER_H

#include <string>
// Ignore interfaces when calculating code coverage
// LCOV_EXCL_START

namespace settingsprovider {
class ISettingIdentifier {
 public:
  enum class Type { INT, STRING };
  ///
  /// \brief ~ISetting
  ///
  virtual ~ISettingIdentifier() {}

  ///
  /// \brief Get the key of the setting
  /// \return The key
  ///
  virtual std::string key() const = 0;

  ///
  /// \brief Get the section of the setting
  /// \return The section
  ///
  virtual std::string section() const = 0;

  ///
  /// \brief Get the underlying type
  /// \return The underlying type
  ///
  virtual Type type() const = 0;
};
// LCOV_EXCL_STOP
}  // namespace settingsprovider
#endif  // ISETTINGIDENTIFIER_H
