#ifndef ISETTING_H
#define ISETTING_H

#include <string>
#include "settingsprovider/isettingidentifier.hpp"
// Ignore interfaces when calculating code coverage
// LCOV_EXCL_START

namespace settingsprovider {
///
/// \brief The setting interface. Provides a read only view
///
template <typename ValueType>
class ISetting : public virtual ISettingIdentifier {
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
