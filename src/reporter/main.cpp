#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include "constants/settings/settingsidentifier.hpp"
#include "core/appsetup.hpp"
#include "core/settings/settingshelper.hpp"
#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "imonitoringplugin/constants.hpp"
#include "imonitoringplugin/iplugindataprocessorcollection.hpp"
#include "persistenceservice/adapterfactory.hpp"
#include "persistenceservice/sqlite/sqlitesettings.hpp"
#include "pluginmanager/include/plugin_manager.hpp"
#include "reporter/templateprocessor/templatetokenfactory.hpp"
#include "templateprocessor/templateprocessor.hpp"

INITIALIZE_EASYLOGGINGPP

using ProcessorLoader =
    pluginmanager::PluginManager<imonitorplugin::IPluginDataProcessorCollection,
                                 imonitorplugin::create_processors_t,
                                 imonitorplugin::destroy_processors_t>;

std::vector<std::string> DiscoverPlugins(const std::string &path,
                                         const std::string &name_filter);

int main() {
  // setup logging
  core::ConfigureLogger();

  // Get settings and data adapter
  auto settings = core::settings::GetApplicationSettings();
  auto adapter_factory = std::make_shared<persistenceservice::AdapterFactory>(
      std::make_unique<persistenceservice::sqlite::SqliteSettings>(
          settings->GetValue(constants::settings::SqliteDatabaseFile())));
  std::shared_ptr<persistenceservice::IDataAdapter> adapter =
      adapter_factory->CreateAdapter();

  // Load data processors from plugins
  ProcessorLoader loader{};
  std::vector<ProcessorLoader::plugin_t *> processor_plugins;
  for (const auto &file : DiscoverPlugins(
           settings->GetValue(constants::settings::PluginBasePath()),
           settings->GetValue(constants::settings::PluginFilter()))) {
    try {
      processor_plugins.push_back(
          loader.LoadPlugin(file, imonitoringplugin::kDataProcessorConstructor,
                            imonitoringplugin::kDataProcessorDestructor));
      LOG(DEBUG) << "Loaded data processors from " << file << " (plugin: "
                 << processor_plugins.back()->Instance()->plugin() << ")";
    } catch (std::exception) {
      LOG(WARNING) << "The plugin library " << file
                   << " does not define any template processors.";
    }
  }

  // Move the actual data into the processors and get the tokens
  reporter::templateprocessor::TemplateTokenFactory token_factory{adapter};
  std::vector<reporter::templateprocessor::TemplateToken> tokens;
  for (const auto &processor : processor_plugins) {
    // TODO pass actual min_age
    auto t = token_factory.BuildTokens(
        processor->Instance(),
        std::stoi(settings->GetValue(constants::settings::DataAge())));
    tokens.insert(tokens.end(), t.begin(), t.end());
  }

  // Run the template processor
  reporter::templateprocessor::TemplateProcessor template_processor{tokens};
  // TODO actually execute the template processor

  // Destroy loaded plugins
  loader.DestroyAll();

  return 0;
}

std::vector<std::string> DiscoverPlugins(const std::string &path,
                                         const std::string &name_filter) {
  LOG(DEBUG) << "Loading plugins from " + path << " using the filter "
             << name_filter;
  std::vector<std::string> files;
  std::regex rgx{name_filter};
  for (const auto &file : std::filesystem::directory_iterator(path)) {
    if (name_filter.empty() ||
        std::regex_match(file.path().filename().string(), rgx)) {
      files.push_back(file.path());
    }
  }
  return files;
}
