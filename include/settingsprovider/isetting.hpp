#ifndef ISETTING_H
#define ISETTING_H

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

///
/// \brief The setting interface. Provides a read only view
///
template <typename ValueType>
class ISetting : public ISettingIdentifier {
 public:
  ///
  /// \brief Get the value of the setting
  /// \return The value
  ///
  virtual ValueType value() const = 0;
};
// LCOV_EXCL_STOP
}  // namespace settingsprovider
#endif  // ISETTING_H
