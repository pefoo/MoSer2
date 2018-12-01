#ifndef SETTINGSPROVIDER_H
#define SETTINGSPROVIDER_H

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "settingsprovider/isetting.hpp"
#include "settingsprovider/setting.hpp"

namespace settingsprovider {

///
/// \brief The settings provider.
/// \details Register settings before actually loading them from a file to setup
/// type constraints
///
class SettingsProvider {
 public:
  ///
  /// \brief Create a new settings provider instance
  ///
  SettingsProvider() = default;

  ///
  /// \brief Get value
  /// \param section The section of the value
  /// \param key The key of the value
  /// \return The value
  ///
  template <typename ValueType>
  ValueType GetValue(std::string key, std::string section) const {
    auto setting =
        dynamic_cast<ISetting<ValueType>*>(this->settings_.Get(key, section));
    if (setting) {
      return setting->value();
    } else {
      throw std::runtime_error("Setting type and identifier do not match.");
    }
  }

  ///
  /// \brief Register a setting
  /// \param s The setting to register
  ///
  template <typename ValueType>
  void RegisterSetting(ISetting<ValueType>* s) {
    this->settings_.Add(std::unique_ptr<ISettingIdentifier>{s});
  }

  ///
  /// \brief Read settings from file
  /// \param file The file to read
  /// \param msg Message output buffer
  /// \return True, if reading succeeded
  ///
  bool ReadFromFile(const std::string file, std::vector<std::string>* msg);

 private:
  ///
  /// \brief The SettingsMap struct
  /// \details Stores settings objects in a hash map using the section and key
  /// as keys.
  ///
  struct SettingsMap {
   public:
    void Add(std::unique_ptr<ISettingIdentifier> setting) {
      this->settings_[this->MakeKey(setting.get())] = std::move(setting);
    }

    ISettingIdentifier* Get(std::string key, std::string section) const {
      return this->settings_.at(this->MakeKey(key, section)).get();
    }

    bool SetValue(std::string key, std::string section,
                  const std::string& value, std::string* msg) {
      auto setting = this->settings_[this->MakeKey(key, section)].get();
      switch (setting->type()) {
        case ISettingIdentifier::Type::INT:
          return this->SetValueInternal<int>(key, section, value, msg);
        case ISettingIdentifier::Type::STRING:
          return this->SetValueInternal<std::string>(key, section, value, msg);
        default:
          throw std::runtime_error("Not supported value type.");
      }
    }

    bool Contains(std::string section, std::string key) const {
      return this->settings_.find(this->MakeKey(key, section)) !=
             this->settings_.end();
    }

   private:
    template <typename ValueType>
    bool SetValueInternal(std::string key, std::string section,
                          const std::string& value, std::string* msg) {
      auto s = dynamic_cast<Setting<ValueType>*>(
          this->settings_[this->MakeKey(key, section)].get());
      if (s) {
        return s->set_value(value, msg);
      } else {
        *msg = "Settings type and identifier do not match!";
        return false;
      }
    }

    std::string MakeKey(ISettingIdentifier* setting) const {
      return this->MakeKey(setting->key(), setting->section());
    }
    std::string MakeKey(const std::string key,
                        const std::string section) const {
      return section + key;
    }

    std::unordered_map<std::string, std::unique_ptr<ISettingIdentifier>>
        settings_;
  };
  SettingsMap settings_;
};
}  // namespace settingsprovider
#endif  // SETTINGSPROVIDER_H
