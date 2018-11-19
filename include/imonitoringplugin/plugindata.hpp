#ifndef PLUGINDATA_H
#define PLUGINDATA_H

#include <string>
#include <utility>
#include <vector>

namespace imonitorplugin {
struct PluginData {
  const std::string PluginName;
  const std::int64_t Timestamp;
  const std::vector<std::pair<std::string, std::string>> Data;
};
}
#endif  // PLUGINDATA_H
