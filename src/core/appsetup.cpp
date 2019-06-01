#include "core/appsetup.hpp"
#include <string>
#include "constants/constants.hpp"
#include "easyloggingpp-9.96.5/easylogging++.h"
#include "utility/filesystem/fileaccesshelper.hpp"

std::string core::GetApplicationConfigFile() {
  auto p = utility::filesystem::GetCurrentExecutablePath();
  auto config_file =
      utility::filesystem::PathCombine({p, constants::kMoser2Conf});
  if (std::filesystem::exists(config_file)) {
    return config_file;
  }
  LOG(ERROR) << "Failed to find the configuration file: " << config_file;
  throw std::runtime_error("Failed to find the configuration file: " +
                           config_file.string());
}

void core::ConfigureLogger() {
  auto p = utility::filesystem::GetCurrentExecutablePath();
  auto config_file =
      utility::filesystem::PathCombine({p, constants::kLoggerConf});
  if (std::filesystem::exists(config_file)) {
    el::Configurations conf(config_file);
    el::Loggers::reconfigureAllLoggers(conf);
    return;
  }
  LOG(ERROR) << "Failed to find the logger configuration file: " << config_file;
  throw std::runtime_error("Failed to find the logger configuration file: " +
                           std::string(config_file));
}
