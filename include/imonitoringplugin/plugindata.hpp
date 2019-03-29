#ifndef PLUGINDATA_H
#define PLUGINDATA_H

#include <functional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "utility/datastructure/Any.hpp"

namespace imonitorplugin {
///
/// \brief The PluginData class
///
class PluginData {
 public:
  using data_vector =
      std::vector<std::pair<std::string, utility::datastructure::Any>>;

  ///
  /// \brief PluginData
  ///
  PluginData(std::string name = "", std::int64_t timestamp = 0,
             data_vector data = {})
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
  data_vector data() const { return this->data_; }

  ///
  /// \brief ToString
  /// \return
  ///
  std::string ToString() const {
    std::stringstream ss;
    ss << this->plugin_name() << std::string(" [")
       << std::to_string(this->timestamp()) << std::string("]: ");
    auto converter =
        this->GetStringConverter(this->data_.front().second.type());
    for (auto& d : this->data()) {
      ss << d.first << std::string("(") << converter(d.second)
         << std::string(");");
    }
    return ss.str();
  }

 private:
  std::function<std::string(utility::datastructure::Any&)> GetStringConverter(
      const std::type_info& type) const {
    if (type == typeid(int)) {
      return [](utility::datastructure::Any& data) -> std::string {
        return std::to_string(data.get<int>());
      };
    } else if (type == typeid(float)) {
      return [](utility::datastructure::Any& data) -> std::string {
        return std::to_string(data.get<float>());
      };
    } else if (type == typeid(double)) {
      return [](utility::datastructure::Any& data) -> std::string {
        return std::to_string(data.get<double>());
      };
    } else if (type == typeid(std::string)) {
      return [](utility::datastructure::Any& data) -> std::string {
        return data.get<std::string>();
      };
    }
    throw std::runtime_error("Not supported data type.");
  }
  std::string plugin_name_;
  std::int64_t timestamp_;
  data_vector data_;
};
}  // namespace imonitorplugin
#endif  // PLUGINDATA_H
