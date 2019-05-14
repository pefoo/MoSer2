#include "core/appsetup.hpp"
#include "constants/constants.hpp"
#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "utility/filesystem/fileaccesshelper.hpp"

std::string core::GetApplicationConfigFile() {
  auto config_file = utility::filesystem::PathCombine(
      {std::filesystem::current_path(), constants::moser2_conf});
  if (std::filesystem::exists(config_file)) {
    return config_file;
  }
  LOG(ERROR) << "Failed to find the configuration file: " << config_file;
  throw std::runtime_error("Failed to find the configuration file: " +
                           config_file.string());
}

void core::ConfigureLogger() {
  if (std::filesystem::exists(constants::logger_conf)) {
    el::Configurations conf(constants::logger_conf);
    el::Loggers::reconfigureAllLoggers(conf);
    return;
  }
  LOG(ERROR) << "Failed to find the logger configuration file: "
             << constants::logger_conf;
  throw std::runtime_error("Failed to find the logger configuration file: " +
                           std::string(constants::logger_conf));
}
