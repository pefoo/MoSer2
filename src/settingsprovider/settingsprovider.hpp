#ifndef SETTINGSPROVIDER_H
#define SETTINGSPROVIDER_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "settingsprovider/isettingsprovider.hpp"
#include "settingsprovider/settingverifier.hpp"

namespace settingsprovider {

class SettingsProvider : public ISettingsProvider {
 public:
  ///
  /// \brief Create a new settings provider instance
  ///
  SettingsProvider() = default;

  ~SettingsProvider() override = default;

  ///
  /// \copydoc ISettingsProvider::GetValue
  ///
  std::string GetValue(const std::string& key,
                       const std::string& section) const override;

  ///
  /// \copydoc ISettingsProvider::GetValue
  ///
  std::string GetValue(const SettingIdentifier& identifier) const override;

  ///
  /// \copydoc ISettingsProvider::Sections
  ///
  std::vector<std::string> Sections() const override;

  ///
  /// \brief Set a setting value
  /// \param key The key of the setting to set
  /// \param section The section of the setting to set
  /// \param value The new value
  /// \return The verifier result. True if everything is fine.
  ///
  bool SetValue(const std::string& key, const std::string& section,
                const std::string& value);

  ///
  /// \brief Register a setting
  /// \param key The setting key
  /// \param section The setting section
  /// \param default_value The default value
  /// \param verifier The verifier to use
  ///
  void RegisterSetting(const std::string& key, const std::string& section,
                       const std::string& default_value = "",
                       const Verifier& verifier = DefaultVerifier);

  ///
  /// \brief Read settings from file
  /// \param file The file to read from
  /// \param msg A message vector. Used to fill with error messages.
  /// \return True, if everything is fine.
  ///
  bool ReadFromFile(const std::string& file, std::vector<std::string>* msg);

  ///
  /// \brief Write the registered settings to a file
  /// \param file The file to write to
  ///
  void WriteToFile(const std::string& file);

 private:
  struct Setting {
    Setting() = default;
    Setting(std::string key, std::string section,
            std::string default_value = "", Verifier verifier = DefaultVerifier)
        : key_(std::move(key)),
          section_(std::move(section)),
          value_(std::move(default_value)),
          verifier_(std::move(verifier)) {}
    std::string key_ = "";
    std::string section_ = "";
    std::string value_ = "";
    Verifier verifier_ = DefaultVerifier;
  };
  std::string BuildKey(const std::string& key,
                       const std::string& section) const;
  std::unordered_map<std::string, Setting> settings_;
};

}  // namespace settingsprovider
#endif  // SETTINGSPROVIDER_H
