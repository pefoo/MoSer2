#include "settingsprovider/settingsprovider.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include "regex"
#include "settingsprovider/settingnotfoundexception.hpp"
#include "settingsprovider/settingverifier.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"
#include "utility/helper/stringhelper.hpp"

namespace settingsprovider {

std::string SettingsProvider::GetValue(const std::string &key,
                                       const std::string &section) const {
  auto map_key = this->BuildKey(key, section);
  if (this->settings_.count(map_key)) {
    return this->settings_.at(this->BuildKey(key, section)).value_;
  } else {
    throw settingsprovider::SettingNotFoundException(key, section);
  }
}

std::string SettingsProvider::GetValue(
    const SettingIdentifier &identifier) const {
  return this->GetValue(identifier.key(), identifier.section());
}

std::vector<std::string> SettingsProvider::Sections() const {
  std::vector<std::string> sections;
  for (auto const &[key, setting] : this->settings_) {
    sections.push_back(setting.section_);
  }
  std::sort(sections.begin(), sections.end());
  auto ip = std::unique(sections.begin(), sections.end());
  sections.erase(ip, sections.end());
  return sections;
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
  return std::string(section + '|' + key);
}

bool SettingsProvider::ReadFromFile(const std::string &file,
                                    std::vector<std::string> *msg) {
  using namespace utility::helper;
  if (!std::filesystem::exists(file)) {
    msg->push_back("The file was not found!");
    return false;
  }
  std::ifstream stream{file};
  std::string line;
  bool result = true;
  std::string current_section;
  std::string section_rgx(R"(^\[(.*)\]\s*$)");
  std::string kv_rgx(R"(^([\w\d\S]+)\s*=\s*([\w\d\S\s]+)\s*$)");
  int line_c = 0;

  while (std::getline(stream, line)) {
    ++line_c;
    if (StringStartsWith(line, "#")) {  // line starts with # -> comment
      continue;
    }
    if (line.empty()) {
      continue;
    }
    std::smatch match;
    if (StringRgxGrep(line, section_rgx, &match)) {  // line is a section header
      current_section = match[1];
    } else if (StringRgxGrep(line, kv_rgx, &match)) {
      // line is a key value pair
      std::string key = match[1];
      std::string value = match[2];
      if (this->settings_.find(this->BuildKey(key, current_section)) !=
          std::end(this->settings_)) {
        if (!this->SetValue(key, current_section, value)) {
          std::string line_msg;
          line_msg.append("Line ")
              .append(std::to_string(line_c))
              .append(": Bad value for ")
              .append(key)
              .append(" (")
              .append(value)
              .append(").");
          msg->push_back(line_msg);
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

void SettingsProvider::WriteToFile(const std::string &file) {
  std::vector<std::string> keys;
  for (const auto &[key, value] : this->settings_) {
    keys.push_back(key);
  }
  // Reverse sort to get keys without a section first
  std::sort(keys.begin(), keys.end(), std::greater<>());

  std::ofstream out{file, std::ofstream::out | std::ofstream::trunc};
  std::string last_section = "";
  for (const auto &k : keys) {
    auto setting = this->settings_.at(k);
    if (last_section != setting.section_) {
      out << "[" << setting.section_ << "]" << std::endl;
    }
    last_section = setting.section_;
    out << setting.key_ << " = " << setting.value_ << std::endl;
  }
}

}  // namespace settingsprovider
