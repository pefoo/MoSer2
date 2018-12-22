#include "monitoringpluginbase/monitorpluginbase.hpp"
#include <ctime>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include "imonitoringplugin/plugindata.hpp"
#include "settingsprovider/isettingsprovider.hpp"
#include "settingsprovider/settingsfactory.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"

monitoringpluginbase::MonitorPluginBase::MonitorPluginBase(std::string name)
    : name_(std::move(name)) {
  if (utility::filesystem::FileExists(this->name() + ".conf")) {
    settingsprovider::SettingsFactory factory{};
    std::vector<std::string> msg;
    this->settings_ = factory.ReadFromFile(this->name() + ".conf", &msg);
  }
}

std::string monitoringpluginbase::MonitorPluginBase::name() const {
  return this->name_;
}

imonitorplugin::PluginData
monitoringpluginbase::MonitorPluginBase::AcquireData() const {
  return imonitorplugin::PluginData{this->name(), this->MakeTimestamp(),
                                    this->AcquireDataInternal()};
}

void monitoringpluginbase::MonitorPluginBase::Sleep100ms() const {
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

std::int64_t monitoringpluginbase::MonitorPluginBase::MakeTimestamp() const {
  return static_cast<std::int64_t>(std::time(nullptr));
}
