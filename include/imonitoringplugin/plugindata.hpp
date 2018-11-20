#ifndef PLUGINDATA_H
#define PLUGINDATA_H

#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace imonitorplugin {
struct PluginData {
  const std::string PluginName;
  const std::int64_t Timestamp;
  const std::vector<std::pair<std::string, std::string>> Data;
  std::string ToString() const {
    std::stringstream ss;
    ss << PluginName << std::string(" [") << std::to_string(Timestamp)
       << std::string("]: ");
    for (const auto& d : Data) {
      ss << d.first << std::string("(") << d.second << std::string(");");
    }
    return ss.str();
  }
};
}
#endif  // PLUGINDATA_H
