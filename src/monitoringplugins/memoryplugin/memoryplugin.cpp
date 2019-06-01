#include "monitoringplugins/memoryplugin/memoryplugin.hpp"
#include <unistd.h>
#include <fstream>
#include <regex>
#include <string>
#include "monitoringplugins/memoryplugin/constants.hpp"

monitoringplugins::memoryplugin::MemoryPlugin::MemoryPlugin()
    : monitoringpluginbase::MonitorPluginBase(constants::kPluginName) {}

imonitorplugin::PluginData::data_vector
monitoringplugins::memoryplugin::MemoryPlugin::AcquireDataInternal(
    imonitorplugin::InputFileContent&& input_file) const {
  const char* meminfo = "/proc/meminfo";
  if (access(meminfo, R_OK) == -1) {
    ThrowPluginException("Failed to access /proc/meminfo");
  }

  std::regex rgx("\\w+:\\s*(\\d+)");
  std::smatch match;

  int total = -1, free = -1, swap_total = -1, swap_free = -1, cached = -1;
  std::ifstream stream(meminfo);
  std::string line;

  while (std::getline(stream, line)) {
    if (total >= 0 && free >= 0 && swap_total >= 0 && swap_free >= 0 &&
        cached >= 0) {
      break;
    } else if (strncmp(line.c_str(), "MemTotal:", strlen("MemTotal")) == 0) {
      if (std::regex_search(line, match, rgx)) {
        total = std::stoi(match[1]);
      } else {
        ThrowPluginException("Failed to parse a meminfo line: " + line);
      }
      continue;
    } else if (strncmp(line.c_str(), "MemFree:", strlen("MemFree")) == 0) {
      if (std::regex_search(line, match, rgx)) {
        free = std::stoi(match[1]);
      } else {
        ThrowPluginException("Failed to parse a meminfo line: " + line);
      }
    } else if (strncmp(line.c_str(), "SwapTotal:", strlen("SwapTotal:")) == 0) {
      if (std::regex_search(line, match, rgx)) {
        swap_total = std::stoi(match[1]);
      } else {
        ThrowPluginException("Failed to parse a meminfo line: " + line);
      }
    } else if (strncmp(line.c_str(), "SwapFree:", strlen("SwapFree:")) == 0) {
      if (std::regex_search(line, match, rgx)) {
        swap_free = std::stoi(match[1]);
      } else {
        ThrowPluginException("Failed to parse a meminfo line: " + line);
      }
    } else if (strncmp(line.c_str(), "Cached", strlen("Cached")) == 0) {
      if (std::regex_search(line, match, rgx)) {
        cached = std::stoi(match[1]);
      } else {
        ThrowPluginException("Failed to parse a meminfo line: " + line);
      }
    }
  }

  int usage_p = int(100 - float(cached + free) / total * 100 + 0.5f);
  int swap_p = int(100 - float(swap_free) / swap_total * 100 + 0.5f);

  imonitorplugin::PluginData::data_vector data;
  data.push_back({"mem_usage", usage_p});
  data.push_back({"swap_usage", swap_p});
  return data;
}
