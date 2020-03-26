#ifndef PLUGINDATA_H
#define PLUGINDATA_H

#include <algorithm>
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
  [[nodiscard]] std::string plugin_name() const { return this->plugin_name_; }

  ///
  /// \brief timestamp
  /// \return
  ///
  [[nodiscard]] std::int64_t timestamp() const { return this->timestamp_; }

  ///
  /// \brief data
  /// \return
  ///
  [[nodiscard]] data_vector data() const { return this->data_; }

  ///
  /// \brief Get a data field
  /// \param name The name of the data field
  /// \return A reference to the requested data field
  ///
  [[nodiscard]] std::any& GetField(const std::string& name) {
    auto field =
        std::find_if(this->data_.begin(), this->data_.end(),
                     [name](const std::pair<std::string, std::any>& v) {
                       return v.first == name;
                     });
    if (field != this->data_.end()) {
      return field->second;
    }
    throw std::runtime_error("Failed to find the data field " + name +
                             " in the data object of " + this->plugin_name());
  }

  ///
  /// \brief Get a data field
  /// \param name The name of the data field
  /// \return The data field value
  ///
  template <typename FieldType>
  [[nodiscard]] FieldType GetFieldValue(const std::string& name) {
    auto field = this->GetField(name);
    return std::any_cast<FieldType>(field);
  }

  ///
  /// \brief ToString
  /// \return
  ///
  [[nodiscard]] std::string ToString() const {
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
  [[nodiscard]] std::function<std::string(std::any&)> GetStringConverter(
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
    }
    if (type == typeid(float)) {
      return [](std::any& data) -> std::string {
        return std::to_string(std::any_cast<float>(data));
      };
    }
    if (type == typeid(double)) {
      return [](std::any& data) -> std::string {
        return std::to_string(std::any_cast<double>(data));
      };
    }
    if (type == typeid(std::string)) {
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
