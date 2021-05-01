#include "monitoringplugins/diskplugin/diskpluginhelper.hpp"
#include <fstream>
#include <streambuf>
#include <regex>

namespace monitoringplugins
{
  namespace diskplugin
  {
    std::string ResolveDevRoot()
    {
      static std::string resolved_device;
      if (!resolved_device.empty())
      {
        return resolved_device;
      }

      std::ifstream is("/proc/cmdline");
      std::string content{(std::istreambuf_iterator<char>(is)),
                          std::istreambuf_iterator<char>()};

      std::regex rgx{"root=(.*?)\\s"};
      std::smatch match;
      if (std::regex_match(content, match, rgx))
      {
        resolved_device = match[1];
      }
      else
      {
        // Might never happen.
        resolved_device = "/dev/root";
      }
      return resolved_device;
    }
  } // namespace diskplugin
} // namespace monitoringplugin