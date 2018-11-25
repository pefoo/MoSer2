#ifndef ISETTING_H
#define ISETTING_H

#include <string>

namespace settingsprovider {

///
/// \brief The setting interface. Provides a read only view
///
template <typename ValueType>
class ISetting {
 public:
  ///
  /// \brief ~ISetting
  ///
  virtual ~ISetting() {}

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
  /// \brief Get the value of the setting
  /// \return The value
  ///
  virtual ValueType value() const = 0;
};
}  // namespace settingsprovider
#endif  // ISETTING_H
