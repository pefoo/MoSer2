#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "constants/settings/settingsidentifier.hpp"
#include "core/appsetup.hpp"
#include "core/settings/settingshelper.hpp"
#include "easyloggingpp-9.96.5/easylogging++.h"
#include "imonitoringplugin/constants.hpp"
#include "imonitoringplugin/iplugindataprocessorcollection.hpp"
#include "persistenceservice/adapterfactory.hpp"
#include "persistenceservice/sqlite/sqlitesettings.hpp"
#include "pluginmanager/pluginmanager.hpp"
#include "reporter/email/emailsender.hpp"
#include "reporter/templateprocessor/templatetokenfactory.hpp"
#include "reporter/tokens/datalesstokenfactory.hpp"
#include "templateprocessor/templateprocessor.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"
#include "utility/helper/stringhelper.hpp"

INITIALIZE_EASYLOGGINGPP

using ProcessorLoader = pluginmanager::PluginManager<
    imonitorplugin::IPluginDataProcessorCollection>;

std::vector<std::string> DiscoverPlugins(const std::string &path,
                                         const std::string &name_filter);
int64_t GetDataMinAge(const std::string &v);

int main() {
  // setup logging
  core::ConfigureLogger();

  if (system("which gnuplot > /dev/null") != 0) {
    LOG(ERROR) << "No gnuplot installation found. Exiting.";
    return 1;
  }

  // Get settings and data adapter
  std::shared_ptr<settingsprovider::ISettingsProvider> settings =
      core::settings::GetApplicationSettings();
  auto adapter_factory = std::make_shared<persistenceservice::AdapterFactory>(
      std::make_unique<persistenceservice::sqlite::SqliteSettings>(
          utility::filesystem::MakeAbsolutePathFromExecutable(
              settings->GetValue(constants::settings::SqliteDatabaseFile()))));
  std::shared_ptr<persistenceservice::IDataAdapter> adapter =
      adapter_factory->CreateAdapter();

  // Load data processors from plugins
  ProcessorLoader loader{};
  std::vector<ProcessorLoader::PluginWrapper *> processor_plugins;
  for (const auto &file : DiscoverPlugins(
           settings->GetValue(constants::settings::PluginBasePath()),
           settings->GetValue(constants::settings::PluginFilter()))) {
    try {
      processor_plugins.push_back(
          loader.LoadPlugin(file, imonitoringplugin::kDataProcessorConstructor,
                            imonitoringplugin::kDataProcessorDestructor));
      LOG(DEBUG) << "Loaded data processors from " << file << " (plugin: "
                 << processor_plugins.back()->Instance()->plugin() << ")";
    } catch (std::exception &) {
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
      try {
        auto t = token_factory.BuildTokens(processor->Instance(), min_age);
        tokens.insert(tokens.end(), t.begin(), t.end());
      } catch (std::runtime_error &re) {
        LOG(ERROR) << "Failed to build the tokens for "
                   << processor->Instance()->plugin() << ". " << re.what();
      }
    }

    // Load the build in tokens
    auto dataless_tokens = reporter::tokens::GetDatalessTokens();
    tokens.insert(tokens.end(), dataless_tokens.begin(), dataless_tokens.end());

    // Run the template processor
    reporter::templateprocessor::TemplateProcessor template_processor{tokens,
                                                                      settings};
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
    std::filesystem::remove(result_file);
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
  auto abs_path = utility::filesystem::MakeAbsolutePathFromExecutable(path);
  for (const auto &file : std::filesystem::directory_iterator(abs_path)) {
    if (name_filter.empty() ||
        utility::helper::StringRgxGrep(file.path().filename().string(),
                                       name_filter)) {
      files.push_back(std::filesystem::canonical(file.path()));
    }
  }
  return files;
}

int64_t GetDataMinAge(const std::string &v) {
  auto now = std::time(nullptr);
  return now - std::stoi(v) * 60;
}
