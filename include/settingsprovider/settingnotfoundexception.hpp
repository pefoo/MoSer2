#ifndef SETTINGNOTFOUNDEXCEPTION_HPP
#define SETTINGNOTFOUNDEXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace settingsprovider {

class SettingNotFoundException : public std::runtime_error {
 public:
  SettingNotFoundException(const std::string& key, const std::string& section);
};

}  // namespace settingsprovider

#endif  // SETTINGNOTFOUNDEXCEPTION_HPP
