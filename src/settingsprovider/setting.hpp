#ifndef SETTING_H
#define SETTING_H

#include <functional>
#include <string>
#include <utility>
#include "settingsprovider/isetting.hpp"

namespace settingsprovider {
///
/// \brief The setting implementation
///
template <typename ValueType>
class SettingBase : public ISetting<ValueType> {
 public:
  using Verifier = std::function<bool(ValueType, std::string* msg)>;
  using Converter = std::function<ValueType(const std::string&)>;

  ///
  /// \brief SettingBase
  /// \param key  The key of the setting
  /// \param section  The section of the setting
  /// \param default_value  The default value for the setting
  /// \param verifier The verifier to apply when setting the value
  /// \param converter  The converter when the setting is created from a string
  ///
  SettingBase(std::string key, std::string section,
              const ValueType& default_value, const Verifier& verifier,
              const Converter& converter)
      : key_(std::move(key)),
        section_(std::move(section)),
        value_(default_value),
        verifier_(verifier),
        string_converter_(converter) {}

  ///
  /// \copydoc ISetting::key()
  ///
  std::string key() const override { return this->key_; }

  ///
  /// \copydoc ISetting::section()
  ///
  std::string section() const override { return this->section_; }

  ///
  /// \copydoc ISetting::value()
  ///
  ValueType value() const override { return this->value_; }

  ///
  /// \brief Set the value of the setting
  /// \param value The new value
  /// \param msg A pointer to a string, filled in case the value is not valid
  /// \return True, if the new value passes the verifier
  ///
  bool set_value(const ValueType& value, std::string* msg) {
    if (this->verifier_(value, msg)) {
      this->value_ = value;
      return true;
    }
    return false;
  }

  ///
  /// \brief Set the value of the setting
  /// \param value The new value
  /// \param msg A pointer to a string, filled in case the value is not valid
  /// \return True, if the new value passes the verifier
  ///
  bool set_value(const std::string value, std::string* msg) {
    return this->set_value(this->string_converter_(value), msg);
  }

 private:
  std::string key_;
  std::string section_;
  ValueType value_;
  Verifier verifier_;
  Converter string_converter_;
};

///
/// \brief Verifiers struct
/// \details Provides the default verifier, that always returns true
///
template <typename ValueType>
struct Verifiers {
  static const std::function<bool(const ValueType&, std::string*)>
      default_verifier;
};
template <typename ValueType>
const std::function<bool(const ValueType&, std::string*)>
    Verifiers<ValueType>::default_verifier =
        [](const ValueType&, std::string*) { return true; };

///
/// \brief Converts struct
/// \details This is the undefined version
///
template <typename ValueType>
struct Converters;

///
/// \brief The string converter
///
template <>
struct Converters<std::string> {
  static const std::function<std::string(const std::string&)> converter;
};
const std::function<std::string(const std::string&)>
    Converters<std::string>::converter =
        [](const std::string& s) -> std::string { return s; };

///
/// \brief The int converter
///
template <>
struct Converters<int> {
  static const std::function<int(const std::string&)> converter;
};
const std::function<int(const std::string&)> Converters<int>::converter =
    [](const std::string& s) -> int { return std::stoi(s); };

///
/// \brief The default Setting class
///
template <typename ValueType>
class Setting : public SettingBase<ValueType> {
 public:
  Setting(std::string key, std::string section, const ValueType& default_value,
          const typename SettingBase<ValueType>::Verifier& verifier,
          const typename SettingBase<ValueType>::Converter& converter)
      : SettingBase<ValueType>(key, section, default_value, verifier,
                               converter) {}
};

///
/// \brief Specialized setting class for string types
///
template <>
class Setting<std::string> : public SettingBase<std::string> {
 public:
  Setting(std::string key, std::string section,
          const std::string& default_value = "",
          const Verifier& verifier = Verifiers<std::string>::default_verifier)
      : SettingBase(key, section, default_value, verifier,
                    Converters<std::string>::converter) {}
};

///
/// \brief Specialized setting class for int types
///
template <>
class Setting<int> : public SettingBase<int> {
 public:
  Setting(std::string key, std::string section, const int& default_value = 0,
          const Verifier& verifier = Verifiers<int>::default_verifier)
      : SettingBase(key, section, default_value, verifier,
                    Converters<int>::converter) {}
};

}  // namespace settingsprovider

#endif  // SETTING_H
