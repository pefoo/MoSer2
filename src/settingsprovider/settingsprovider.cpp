#include "settingsprovider/settingsprovider.hpp"
#include <fstream>
#include <string>
#include <vector>
#include "regex"
#include "settingsprovider/settingverifier.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"

namespace settingsprovider {

std::string SettingsProvider::GetValue(const std::string &key,
                                       const std::string &section) const {
  return this->settings_.at(this->BuildKey(key, section)).value_;
}

std::string SettingsProvider::GetValue(
    const SettingIdentifier &identifier) const {
  return this->GetValue(identifier.key(), identifier.section());
}

bool SettingsProvider::SetValue(const std::string &key,
                                const std::string &section,
                                const std::string &value) {
  auto setting = this->settings_.find(this->BuildKey(key, section));
  if (setting != std::end(this->settings_)) {
    if (setting->second.verifier_(value)) {
      setting->second.value_ = value;
      return true;
    }
  }
  return false;
}

void SettingsProvider::RegisterSetting(const std::string &key,
                                       const std::string &section,
                                       const std::string &default_value,
                                       const Verifier &verifier) {
  this->settings_[this->BuildKey(key, section)] =
      Setting(key, section, default_value, verifier);
}

std::string SettingsProvider::BuildKey(const std::string &key,
                                       const std::string &section) const {
  return std::string(key + section);
}

bool SettingsProvider::ReadFromFile(const std::string &file,
                                    std::vector<std::string> *msg) {
  if (!utility::filesystem::FileExists(file)) {
    msg->push_back("The file was not found!");
    return false;
  }
  std::ifstream stream{file};
  std::string line;
  bool result = true;
  std::string current_section = "";
  std::regex section_rgx("^\\[(.*)\\]\\s*$");
  std::regex kv_rgx("^([\\w\\d\\S]+)\\s*=\\s*([\\w\\d\\S]+)\\s*$");
  int line_c = 0;

  while (std::getline(stream, line)) {
    ++line_c;
    std::smatch match;
    if (line.find("#") == 0) {  // line starts with # -> comment
      continue;
    } else if (std::regex_match(line, match,
                                section_rgx)) {  // line is a section header
      current_section = match[1];
      continue;
    } else if (std::regex_match(line, match,
                                kv_rgx)) {  // line is a key value pair
      std::string key = match[1];
      std::string value = match[2];
      if (this->settings_.find(this->BuildKey(key, current_section)) !=
          std::end(this->settings_)) {
        if (!this->SetValue(key, current_section, value)) {
          msg->push_back("Line " + std::to_string(line_c) + ": Bad value for " +
                         key + " (" + value + ").");
          return false;
        }
      } else {
        this->RegisterSetting(key, current_section, value);
      }
    } else {
      msg->push_back("Failed to parse a line! " + line);
      result = false;
    }
  }

  return result;
}

}  // namespace settingsprovider
