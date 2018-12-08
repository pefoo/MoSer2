#ifndef SETTINGIDENTIFIER_H
#define SETTINGIDENTIFIER_H

#include <string>
#include "settingsprovider/isettingidentifier.hpp"

namespace settingsprovider {
///
/// \brief The identifier for a setting
///
class SettingIdentifier : public virtual ISettingIdentifier {
 public:
  SettingIdentifier(std::string key, std::string section,
                    Type type = Type::STRING);

  ~SettingIdentifier() override;
  std::string key() const override;
  std::string section() const override;
  Type type() const override;

 private:
  std::string key_;
  std::string section_;
  Type type_;
};

}  // namespace settingsprovider
#endif  // SETTINGIDENTIFIER_H
