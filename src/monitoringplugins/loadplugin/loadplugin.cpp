#include "monitoringplugins/loadplugin/loadplugin.hpp"
#include <unistd.h>
#include <fstream>
#include <regex>
#include <streambuf>
#include <string>
#include <utility>

monitoringplugins::loadplugin::LoadPlugin::LoadPlugin()
    : monitoringpluginbase::MonitorPluginBase("LoadPlugin") {}

imonitorplugin::PluginData::data_vector
monitoringplugins::loadplugin::LoadPlugin::AcquireDataInternal(
    imonitorplugin::InputFileContent&& input_file) const {
  std::string proc_loadavg = "/proc/loadavg";
  if (access(proc_loadavg.c_str(), R_OK) == -1) {
    this->ThrowPluginException("Failed to read from /proc/loadavg");
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
    return {
        {"m1", std::move(m1)}, {"m5", std::move(m5)}, {"m15", std::move(m15)}};
  } else {
    this->ThrowPluginException(
        "Failed to parse content of /proc/loadavg. Content: " + content);
  }
}
