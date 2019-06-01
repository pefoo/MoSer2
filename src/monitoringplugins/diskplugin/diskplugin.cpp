#include "monitoringplugins/diskplugin/diskplugin.hpp"
#include <unistd.h>
#include <fstream>
#include <regex>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include "monitoringplugins/diskplugin/constants.hpp"

monitoringplugins::diskplugin::DiskPlugin::DiskPlugin()
    : monitoringpluginbase::MonitorPluginBase(constants::kPluginName) {
  this->RegisterFileToRead("/proc/diskstats");
}

imonitorplugin::PluginData::data_vector
monitoringplugins::diskplugin::DiskPlugin::AcquireDataInternal(
    imonitorplugin::InputFileContent &&input_file) const {}
