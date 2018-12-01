#include "settingsprovider/settingsprovider.hpp"
#include <fstream>
#include <string>
#include <vector>
#include "regex"
#include "utility/filesystem/fileaccesshelper.hpp"

namespace settingsprovider {

bool SettingsProvider::ReadFromFile(const std::string file,
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

  while (std::getline(stream, line)) {
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
      if (this->settings_.Contains(current_section, key)) {
        std::string error;
        if (!this->settings_.SetValue(key, current_section, value, &error)) {
          msg->push_back(error);
          return false;
        }
      } else {
        this->RegisterSetting(
            new Setting<std::string>{key, current_section, value});
      }
    } else {
      msg->push_back("Failed to parse a line! " + line);
      result = false;
    }
  }

  return result;
}

}  // namespace settingsprovider
