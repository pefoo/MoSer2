#include <ctime>
#include <string>
#include <utility>
#include "imonitoringplugin/plugindata.hpp"
#include "monitoringpluginbase/monitorpluginbase.hpp"

monitoringpluginbase::MonitorPluginBase::MonitorPluginBase(std::string name)
    : name_(std::move(name)) {}

std::string monitoringpluginbase::MonitorPluginBase::name() const {
  return this->name_;
}

imonitorplugin::PluginData
monitoringpluginbase::MonitorPluginBase::AcquireData() const {
  return imonitorplugin::PluginData{this->name(), this->MakeTimestamp(),
                                    this->AcquireDataInternal()};
}

void monitoringpluginbase::MonitorPluginBase::Sleep100ms() const {
  struct timespec dur = {0, 0};
  dur.tv_sec = 0;
  dur.tv_nsec = 100000000;
  nanosleep(&dur, (struct timespec*)nullptr);
}

std::int64_t monitoringpluginbase::MonitorPluginBase::MakeTimestamp() const {
  return static_cast<std::int64_t>(std::time(nullptr));
}
