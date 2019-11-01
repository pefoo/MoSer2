#include "monitoringpluginbase/monitorpluginbase.hpp"
#include <filesystem>
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
