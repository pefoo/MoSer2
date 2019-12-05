#include "monitoringplugins/memoryplugin/memoryplugin.hpp"
#include <unistd.h>
#include <fstream>
#include <regex>
#include <string>
#include "monitoringplugins/memoryplugin/constants.hpp"
#include "utility/helper/stringhelper.hpp"

monitoringplugins::memoryplugin::MemoryPlugin::MemoryPlugin()
    : monitoringpluginbase::MonitorPluginBase(constants::kPluginName) {}

imonitorplugin::PluginData::data_vector
monitoringplugins::memoryplugin::MemoryPlugin::AcquireDataInternal(
    [[maybe_unused]] std::unordered_map<
        std::string, imonitorplugin::InputFileContent>&& input_file) {
  const char* meminfo = "/proc/meminfo";
  if (access(meminfo, R_OK) == -1) {
    ThrowPluginException("Failed to access /proc/meminfo");
  }

  std::regex rgx(R"(\w+:\s*(\d+))");
  std::smatch match;

  int total = -1, free = -1, swap_total = -1, swap_free = -1, cached = -1;
  std::ifstream stream(meminfo);
  std::string line;

  while (std::getline(stream, line)) {
    if (total >= 0 && free >= 0 && swap_total >= 0 && swap_free >= 0 &&
        cached >= 0) {
      break;
    }
    using namespace utility::helper;
    if (StringStartsWith(line, "MemTotal")) {
      if (std::regex_search(line, match, rgx)) {
        total = std::stoi(match[1]);
      } else {
        ThrowPluginException("Failed to parse a meminfo line: " + line);
      }
      continue;
    } else if (StringStartsWith(line, "MemFree:")) {
      if (std::regex_search(line, match, rgx)) {
        free = std::stoi(match[1]);
      } else {
        ThrowPluginException("Failed to parse a meminfo line: " + line);
      }
    } else if (StringStartsWith(line, "SwapTotal:")) {
      if (std::regex_search(line, match, rgx)) {
        swap_total = std::stoi(match[1]);
      } else {
        ThrowPluginException("Failed to parse a meminfo line: " + line);
      }
    } else if (StringStartsWith(line, "SwapFree:")) {
      if (std::regex_search(line, match, rgx)) {
        swap_free = std::stoi(match[1]);
      } else {
        ThrowPluginException("Failed to parse a meminfo line: " + line);
      }
    } else if (StringStartsWith(line, "Cached")) {
      if (std::regex_search(line, match, rgx)) {
        cached = std::stoi(match[1]);
      } else {
        ThrowPluginException("Failed to parse a meminfo line: " + line);
      }
    }
  }

  int usage_p = static_cast<int>(
      100 - static_cast<float>(cached + free) / total * 100 + 0.5f);
  int swap_p = static_cast<int>(
      100 - static_cast<float>(swap_free) / swap_total * 100 + 0.5f);

  imonitorplugin::PluginData::data_vector data;
  data.push_back({"mem_usage", usage_p});
  data.push_back({"swap_usage", swap_p});
  return data;
}
