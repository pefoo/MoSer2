#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "constants/constants.hpp"
#include "constants/settings/settingsidentifier.hpp"
#include "core/appsetup.hpp"
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

std::unique_ptr<settingsprovider::ISettingsProvider> GetSettings();

int main() {
  // Setup the logger
  core::ConfigureLogger();

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
/// \brief Find and read the settings file
/// \return The settings object
///
std::unique_ptr<settingsprovider::ISettingsProvider> GetSettings() {
  auto settings_factory = moser2::settings::RegisterFactory();
  std::vector<std::string> errors;
  auto config_file = core::GetConfigFile();
  LOG(DEBUG) << "Reading settings from " << config_file;
  auto settings = settings_factory->ReadFromFile(config_file, &errors);

  if (!errors.empty()) {
    for (const auto& e : errors) {
      LOG(ERROR) << e;
    }
    throw std::runtime_error("Detected invalid configuration. See " +
                             std::string(constants::logger_conf) +
                             " for more information.");
  }
  return settings;
}
