#include <memory>
#include <string>
#include "constants/settings/settingsidentifier.hpp"
#include "core/appsetup.hpp"
#include "core/settings/settingshelper.hpp"
#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "persistenceservice/adapterfactory.hpp"
#include "persistenceservice/sqlite/sqlitesettings.hpp"

INITIALIZE_EASYLOGGINGPP

int main() {
  core::ConfigureLogger();
  auto settings = core::settings::GetApplicationSettings();
  auto adapter_factory = std::make_shared<persistenceservice::AdapterFactory>(
      std::make_unique<persistenceservice::sqlite::SqliteSettings>(
          settings->GetValue(constants::settings::SqliteDatabaseFile())));
  auto adapter = adapter_factory->CreateAdapter();

  return 0;
}
