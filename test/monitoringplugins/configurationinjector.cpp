#include "configurationinjector.hpp"
#include <filesystem>
#include <sstream>
#include <string>

ConfigurationInjector::ConfigurationInjector(std::string plugin_name)
    : plugin_name_(plugin_name) {
  std::stringstream s1{};
  s1 << LIBRARY_DIR << "/" << plugin_name << ".conf";
  std::stringstream s2{};
  s2 << TESTDATA_DIR << "/" << plugin_name << ".conf";

  this->config_file_ = s1.str();
  this->config_backup_ = this->config_file_ + ".bak";
  auto testdata_config = s2.str();

  if (std::filesystem::exists(this->config_file_)) {
    std::filesystem::copy_file(
        this->config_file_, this->config_backup_,
        std::filesystem::copy_options::overwrite_existing);
  }

  std::filesystem::copy_file(testdata_config, this->config_file_,
                             std::filesystem::copy_options::overwrite_existing);
}

ConfigurationInjector::~ConfigurationInjector() {
  if (std::filesystem::exists(this->config_backup_)) {
    std::filesystem::copy_file(
        this->config_backup_, this->config_file_,
        std::filesystem::copy_options::overwrite_existing);
  }
}
