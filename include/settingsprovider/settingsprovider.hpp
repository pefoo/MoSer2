#ifndef SETTINGSPROVIDER_H
#define SETTINGSPROVIDER_H

#include <string>
#include "settingsprovider/setting.hpp"

namespace settingsprovider {

///
/// \brief TODO
///
class SettingsProvider {
 public:
  ///
  /// \brief Create a new settings provider instance using the provided file
  /// \param file The settings file to read
  ///
  explicit SettingsProvider(const std::string& file);

 private:
};
}  // namespace settingsprovider
#endif  // SETTINGSPROVIDER_H
