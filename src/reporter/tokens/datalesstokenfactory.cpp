#include "datalesstokenfactory.hpp"
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <ctime>
#include <sstream>
#include <tuple>

std::vector<reporter::templateprocessor::TemplateToken>
reporter::tokens::GetDatalessTokens() {
  struct utsname info;
  uname(&info);
  return std::vector<reporter::templateprocessor::TemplateToken>{
      CREATE_TOKEN("%%UPTIME%%",
                   {
                     struct sysinfo info;
                     if (sysinfo(&info) != 0) {
                       return "";
                     }
                     long total_seconds = info.uptime;
                     long days = total_seconds / 86400;
                     long hours = (total_seconds % 86400) / 3600;
                     long minutes = (total_seconds % 3600) / 60;
                     long seconds = total_seconds % 60;
                     std::stringstream s;
                     if (days > 0) s << days << " days ";
                     if (hours > 0) s << hours << " hours ";
                     if (minutes > 0) s << minutes << " minutes ";
                     s << seconds << " seconds";
                     return s.str();
                   }),
      CREATE_TOKEN("%%DATETIME%%",
                   {
                     auto t = std::time(nullptr);
                     return std::ctime(&t);
                   }),
      CREATE_TOKEN_CAPTURE("%%HOSTNAME%%", info, { return info.nodename; }),
      CREATE_TOKEN_CAPTURE("%%KERNEL_VERSION%%", info,
                           { return info.release; }),
      CREATE_TOKEN_CAPTURE("%%SYSTEM_NAME%%", info, { return info.sysname; }),
      CREATE_TOKEN_CAPTURE("%%MACHINE_ARCH%%", info, { return info.machine; }),
      CREATE_TOKEN_CAPTURE("%%DOMAIN_NAME%%", info,
                           { return info.domainname; })};
}
