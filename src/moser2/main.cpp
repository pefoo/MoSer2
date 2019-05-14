#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "constants/settings/settingsidentifier.hpp"
#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "monitoringserver.hpp"
#include "persistenceservice/adapterfactory.hpp"
#include "persistenceservice/persistencerunner.hpp"
#include "persistenceservice/sqlite/sqlitesettings.hpp"
#include "plugin/pluginfacade.hpp"
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
  // Setup the logger
  ConfigureLogger();

  // Read the settings
  std::shared_ptr<settingsprovider::ISettingsProvider> settings = GetSettings();

  // Start the monitoring server
  LOG(DEBUG) << "Starting the monitoring server";
  moser2::MonitoringServer server{settings};
  server.Run();

  // Create the default data adapter
  LOG(DEBUG) << "Creating sqlite3 adapter";
  auto adapter_factory = new persistenceservice::AdapterFactory(
      std::make_unique<persistenceservice::sqlite::SqliteSettings>(
          settings->GetValue(constants::settings::SqliteDatabaseFile())));
  auto adapter = adapter_factory->CreateAdapter();

  // Start the persistence runner
  LOG(DEBUG) << "Starting the persistence service";
  persistenceservice::PersistenceRunner p_runner{
      std::move(adapter), std::bind(&moser2::plugin::PluginFacade::GetNext,
                                    &moser2::plugin::PluginFacade::Instance(),
                                    std::placeholders::_1)};
  p_runner.Run(10000);

  getchar();

  // Sop running servers
  LOG(DEBUG) << "Stopping the persistence service";
  p_runner.Stop();
  LOG(DEBUG) << "Stopping the monitoring server";
  server.Stop();
  return 0;
}

///
/// \brief Get the default config file.
/// \return The default config file
///
std::string GetConfigFile() {
  auto config_file = utility::filesystem::PathCombine(
      {std::filesystem::current_path(), "/", moser2_conf});
  if (std::filesystem::exists(config_file)) {
    return config_file;
  }
  LOG(ERROR) << "Failed to find the configuration file: " << config_file;
  throw std::runtime_error("Failed to find the configuration file: " +
                           config_file.string());
}

///
/// \brief Configure the logger
///
void ConfigureLogger() {
  if (std::filesystem::exists(logger_conf)) {
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
  LOG(DEBUG) << "Reading settings from " << config_file;
  auto settings = settings_factory->ReadFromFile(config_file, &errors);

  if (!errors.empty()) {
    for (const auto& e : errors) {
      LOG(ERROR) << e;
    }
    throw std::runtime_error("Detected invalid configuration. See " +
                             std::string(logger_conf) +
                             " for more information.");
  }
  return settings;
}
