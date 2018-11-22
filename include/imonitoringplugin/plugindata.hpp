#ifndef PLUGINDATA_H
#define PLUGINDATA_H

#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace imonitorplugin {
///
/// \brief The PluginData class
///
class PluginData {
 public:
  ///
  /// \brief PluginData
  ///
  PluginData(std::string name = "", std::int64_t timestamp = 0,
             std::vector<std::pair<std::string, std::string>> data = {})
      : plugin_name_(name), timestamp_(timestamp), data_(data) {}

  ///
  /// \brief plugin_name
  /// \return
  ///
  std::string plugin_name() const { return this->plugin_name_; }

  ///
  /// \brief timestamp
  /// \return
  ///
  std::int64_t timestamp() const { return this->timestamp_; }

  ///
  /// \brief data
  /// \return
  ///
  std::vector<std::pair<std::string, std::string>> data() const {
    return this->data_;
  }

  ///
  /// \brief ToString
  /// \return
  ///
  std::string ToString() const {
    std::stringstream ss;
    ss << this->plugin_name() << std::string(" [")
       << std::to_string(this->timestamp()) << std::string("]: ");
    for (const auto& d : this->data()) {
      ss << d.first << std::string("(") << d.second << std::string(");");
    }
    return ss.str();
  }

 private:
  std::string plugin_name_;
  std::int64_t timestamp_;
  std::vector<std::pair<std::string, std::string>> data_;
};
}  // namespace imonitorplugin
#endif  // PLUGINDATA_H
