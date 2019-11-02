#ifndef PLUGINDATA_H
#define PLUGINDATA_H

#include <any>
#include <functional>
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
  // TODO this data vector is shit
  using data_vector = std::vector<std::pair<std::string, std::any>>;

  ///
  /// \brief PluginData
  ///
  explicit PluginData(std::string name = "", std::int64_t timestamp = 0,
                      data_vector data = {})
      : plugin_name_(std::move(name)),
        timestamp_(timestamp),
        data_(std::move(data)) {}

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
    for (auto& d : this->data()) {
      ss << d.first << std::string("(")
         << this->GetStringConverter(d.second.type())(d.second)
         << std::string(");");
    }
    return ss.str();
  }

 private:
  std::function<std::string(std::any&)> GetStringConverter(
      const std::type_info& type) const {
    if (type == typeid(int)) {
      return [](std::any& data) -> std::string {
        return std::to_string(std::any_cast<int>(data));
      };
    }
    if (type == typeid(int64_t)) {
      return [](std::any& data) -> std::string {
        return std::to_string(std::any_cast<int64_t>(data));
      };
    } else if (type == typeid(float)) {
      return [](std::any& data) -> std::string {
        return std::to_string(std::any_cast<float>(data));
      };
    } else if (type == typeid(double)) {
      return [](std::any& data) -> std::string {
        return std::to_string(std::any_cast<double>(data));
      };
    } else if (type == typeid(std::string)) {
      return [](std::any& data) -> std::string {
        return std::any_cast<std::string>(data);
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
