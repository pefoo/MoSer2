#include "monitoringplugins/loadplugin/loadplugin.hpp"
#include <unistd.h>
#include <fstream>
#include <regex>
#include <streambuf>
#include <string>

monitoringplugins::loadplugin::LoadPlugin::LoadPlugin()
    : monitoringpluginbase::MonitorPluginBase("LoadPlugin") {}

imonitorplugin::PluginData::data_vector
monitoringplugins::loadplugin::LoadPlugin::AcquireDataInternal() const {
  std::string proc_loadavg = "/proc/loadavg";
  if (access(proc_loadavg.c_str(), R_OK) == -1) {
    // TODO Error handling
  }

  std::ifstream stream(proc_loadavg);
  std::stringstream buffer;
  buffer << stream.rdbuf();
  std::string content = buffer.str();

  std::regex rgx{R"((\d+.\d+) (\d+\.\d+) (\d+\.\d+).*\n)"};
  std::smatch match;

  if (std::regex_match(content, match, rgx)) {
    float m1 = std::stof(match[1]);
    float m5 = std::stof(match[2]);
    float m15 = std::stof(match[3]);
    return {{"m1", utility::datastructure::Any{std::move(m1)}},
            {"m5", utility::datastructure::Any{std::move(m5)}},
            {"m15", utility::datastructure::Any{std::move(m15)}}};
  } else {
    // TODO error handling
  }
}
