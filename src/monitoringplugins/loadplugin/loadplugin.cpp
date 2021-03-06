#include "monitoringplugins/loadplugin/loadplugin.hpp"
#include <unistd.h>
#include <fstream>
#include <regex>
#include <streambuf>
#include <string>
#include <utility>
#include "utility/helper/stringhelper.hpp"

monitoringplugins::loadplugin::LoadPlugin::LoadPlugin()
    : monitoringpluginbase::MonitorPluginBase("LoadPlugin") {}

imonitorplugin::PluginData::data_vector
monitoringplugins::loadplugin::LoadPlugin::AcquireDataInternal(
    [[maybe_unused]] std::unordered_map<
        std::string, imonitorplugin::InputFileContent> &&input_file) {
  std::string proc_loadavg = "/proc/loadavg";
  if (access(proc_loadavg.c_str(), R_OK) == -1) {
    this->ThrowPluginException("Failed to read from /proc/loadavg");
  }

  std::ifstream stream(proc_loadavg);
  std::stringstream buffer;
  buffer << stream.rdbuf();
  std::string content = buffer.str();

  std::string rgx{R"((\d+.\d+) (\d+\.\d+) (\d+\.\d+).*\n)"};
  std::smatch match;

  if (utility::helper::StringRgxGrep(content, rgx, &match)) {
    float m1 = std::stof(match[1]);
    float m5 = std::stof(match[2]);
    float m15 = std::stof(match[3]);
    return {{"m1", m1}, {"m5", m5}, {"m15", m15}};
  } else {
    this->ThrowPluginException(
        "Failed to parse content of /proc/loadavg. Content: " + content);
  }
}
