#include <memory>
#include <string>
#include <utility>
#include "constants/settings/settingsidentifier.hpp"
#include "core/appsetup.hpp"
#include "core/settings/settingshelper.hpp"
#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "monitoringserver.hpp"
#include "persistenceservice/adapterfactory.hpp"
#include "persistenceservice/persistencerunner.hpp"
#include "persistenceservice/sqlite/sqlitesettings.hpp"
#include "plugin/pluginfacade.hpp"
#include "settingsprovider/isettingsprovider.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"

INITIALIZE_EASYLOGGINGPP

int main() {
  // Setup the logger
  core::ConfigureLogger();

  // Read the settings
  std::shared_ptr<settingsprovider::ISettingsProvider> settings =
      core::settings::GetApplicationSettings();

  // Start the monitoring server
  LOG(DEBUG) << "Starting the monitoring server";
  moser2::MonitoringServer server{settings};
  server.Run();

  // Create the default data adapter
  LOG(DEBUG) << "Creating sqlite3 adapter";
  auto adapter_factory = new persistenceservice::AdapterFactory(
      std::make_unique<persistenceservice::sqlite::SqliteSettings>(
          utility::filesystem::MakeAbsolutePathFromExecutable(
              settings->GetValue(constants::settings::SqliteDatabaseFile()))));
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
