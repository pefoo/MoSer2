#include <cstdio>
#include <memory>
#include <string>
#include <vector>
#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "monitoringserver.hpp"
#include "settings/settingshelper.hpp"
#include "settingsprovider/isettingsprovider.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"

INITIALIZE_EASYLOGGINGPP

static const char logger_conf[] = "logger.conf";
static const char moser2_conf[] = "moser2.conf";

std::string GetConfigFile();
void ConfigureLogger();
std::unique_ptr<settingsprovider::ISettingsProvider> GetSettings();

int main() {
  ConfigureLogger();
  moser2::MonitoringServer server{GetSettings()};
  server.Run();
  getchar();
  server.Stop();
  return 0;
}

///
/// \brief Get the default config file.
/// \return The default config file
///
std::string GetConfigFile() {
  auto config_file = utility::filesystem::PathCombine(
      {utility::filesystem::GetCurrentWorkingDir(), "/", moser2_conf});
  if (utility::filesystem::FileExists(config_file)) {
    return config_file;
  }
  LOG(ERROR) << "Failed to find the configuration file: " << config_file;
  throw std::runtime_error("Failed to find the configuration file: " +
                           config_file);
}

///
/// \brief Configure the logger
///
void ConfigureLogger() {
  if (utility::filesystem::FileExists(logger_conf)) {
    el::Configurations conf(logger_conf);
    el::Loggers::reconfigureAllLoggers(conf);
    return;
  }
  LOG(ERROR) << "Failed to find the logger configuration file: " << logger_conf;
  throw std::runtime_error("Failed to find the logger configuration file: " +
                           std::string(logger_conf));
}

///
/// \brief Find and read the settings file
/// \return The settings object
///
std::unique_ptr<settingsprovider::ISettingsProvider> GetSettings() {
  auto settings_factory = moser2::settings::RegisterFactory();
  std::vector<std::string> errors;
  auto config_file = GetConfigFile();
  auto settings = settings_factory->ReadFromFile(config_file, &errors);

  if (errors.size() != 0) {
    for (const auto& e : errors) {
      LOG(ERROR) << e;
    }
    throw std::runtime_error("Detected invalid configuration. See " +
                             std::string(logger_conf) +
                             " for more information.");
  }
  return settings;
}
