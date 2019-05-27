#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "constants/settings/settingsidentifier.hpp"
#include "core/appsetup.hpp"
#include "core/settings/settingshelper.hpp"
#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "imonitoringplugin/constants.hpp"
#include "imonitoringplugin/iplugindataprocessorcollection.hpp"
#include "persistenceservice/adapterfactory.hpp"
#include "persistenceservice/sqlite/sqlitesettings.hpp"
#include "pluginmanager/include/plugin_manager.hpp"
#include "reporter/email/emailsender.hpp"
#include "reporter/templateprocessor/templatetokenfactory.hpp"
#include "templateprocessor/templateprocessor.hpp"

INITIALIZE_EASYLOGGINGPP

using ProcessorLoader =
    pluginmanager::PluginManager<imonitorplugin::IPluginDataProcessorCollection,
                                 imonitorplugin::create_processors_t,
                                 imonitorplugin::destroy_processors_t>;

std::vector<std::string> DiscoverPlugins(const std::string &path,
                                         const std::string &name_filter);
int64_t GetDataMinAge(const std::string &v);

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

  // Make sure anything that depends on plugins is freed before the plugins are
  // closed (DestroyAll() is called)
  {
    // Move the actual data into the processors and get the tokens
    reporter::templateprocessor::TemplateTokenFactory token_factory{adapter};
    std::vector<reporter::templateprocessor::TemplateToken> tokens;
    for (const auto &processor : processor_plugins) {
      auto min_age =
          GetDataMinAge(settings->GetValue(constants::settings::DataAge()));
      auto t = token_factory.BuildTokens(processor->Instance(), min_age);
      tokens.insert(tokens.end(), t.begin(), t.end());
    }

    // Run the template processor
    reporter::templateprocessor::TemplateProcessor template_processor{tokens};
    auto result_file = template_processor.ProcessTemplate(
        settings->GetValue(constants::settings::ReporTemplate()));
    LOG(DEBUG) << "Report file was written to " << result_file;
    if (!reporter::email::SendReport(
            result_file,
            settings->GetValue(constants::settings::MailRecipient()),
            settings->GetValue(constants::settings::SmtpServer()),
            settings->GetValue(constants::settings::MailUser()),
            settings->GetValue(constants::settings::MailPassword()))) {
      LOG(ERROR) << "Failed to send the report";
    }
  }

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
      files.push_back(std::filesystem::canonical(file.path()));
    }
  }
  return files;
}

int64_t GetDataMinAge(const std::string &v) {
  auto now = std::time(nullptr);
  return now - std::stoi(v) * 60;
}
