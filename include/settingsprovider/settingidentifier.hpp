#ifndef SETTINGIDENTIFIER_H
#define SETTINGIDENTIFIER_H

#include <string>

namespace settingsprovider {
///
/// \brief The identifier for a setting
///
class SettingIdentifier {
 public:
  SettingIdentifier(std::string key, std::string section);

  ~SettingIdentifier();
  [[nodiscard]] std::string key() const;
  [[nodiscard]] std::string section() const;

 private:
  std::string key_;
  std::string section_;
};

}  // namespace settingsprovider
#endif  // SETTINGIDENTIFIER_H
