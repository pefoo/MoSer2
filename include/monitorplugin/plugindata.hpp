#ifndef PLUGINDATA_H
#define PLUGINDATA_H

#include <string>
#include <vector>

namespace monitorplugin {
struct PluginData {
  const std::string PluginName;
  const std::int64_t Timestamp;
  const std::vector<std::pair<std::string, std::string>> Data;
};
}
#endif  // PLUGINDATA_H
