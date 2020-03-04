#include "monitoringpluginbase/monitorpluginbase.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include "imonitoringplugin/plugindata.hpp"
#include "imonitoringplugin/pluginexception.hpp"
#include "settingsprovider/isettingsprovider.hpp"
#include "settingsprovider/settingsfactory.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"

monitoringpluginbase::MonitorPluginBase::MonitorPluginBase(std::string name)
    : name_(std::move(name)), input_files_{} {
  if (std::filesystem::exists(this->name() + ".conf")) {
    settingsprovider::SettingsFactory factory{};
    std::vector<std::string> msg;
    this->settings_ = factory.ReadFromFile(this->name() + ".conf", &msg);
  }
}

void monitoringpluginbase::MonitorPluginBase::Init() {}

std::string monitoringpluginbase::MonitorPluginBase::name() const {
  return this->name_;
}

std::vector<std::string> monitoringpluginbase::MonitorPluginBase::input_files()
    const {
  return this->input_files_;
}

std::vector<imonitorplugin::PluginData>
monitoringpluginbase::MonitorPluginBase::AcquireData(
    std::unordered_map<std::string, imonitorplugin::InputFileContent>&&
        input_file) {
  return {imonitorplugin::PluginData{
      this->name(), this->MakeTimestamp(),
      this->AcquireDataInternal(std::move(input_file))}};
}

std::vector<std::string>
monitoringpluginbase::MonitorPluginBase::DoSanityCheck() const {
  return {};
}

void monitoringpluginbase::MonitorPluginBase::Configure() {
  std::string config_file_name = this->name() + ".conf";
  if (std::filesystem::exists(config_file_name)) {
    std::filesystem::copy(config_file_name, config_file_name + ".bak",
                          std::filesystem::copy_options::overwrite_existing);
  }
  settingsprovider::SettingsFactory factory{};
  auto selectors = GetConfigSelectors(std::cout, std::cin);
  if (!selectors.empty()) {
    std::cout << "##################################################"
              << std::endl;
    std::cout << this->name() << std::endl;
    std::cout << "##################################################"
              << std::endl;
    for (const auto& selector : selectors) {
      auto [key, section, value] = selector->SelectConfig();
    }
    factory.WriteToFile(config_file_name);
  }
  std::vector<std::string> msg;
  this->settings_ = factory.ReadFromFile(config_file_name, &msg);
}

std::vector<std::shared_ptr<imonitorplugin::IPluginConfigSelector> >
monitoringpluginbase::MonitorPluginBase::GetConfigSelectors(
    std::ostream&, std::istream&) const {
  return {};
}

void monitoringpluginbase::MonitorPluginBase::RegisterFileToRead(
    const std::string& file) {
  this->input_files_.push_back(file);
}

void monitoringpluginbase::MonitorPluginBase::ThrowPluginException(
    const std::string& msg) const {
  throw imonitorplugin::PluginException(this->name(), msg.c_str());
}

std::int64_t monitoringpluginbase::MonitorPluginBase::MakeTimestamp() const {
  return static_cast<std::int64_t>(std::time(nullptr));
}
