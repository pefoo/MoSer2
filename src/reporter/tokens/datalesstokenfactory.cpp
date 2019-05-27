#include "datalesstokenfactory.hpp"
#include <sys/utsname.h>
#include <vector>

std::vector<reporter::templateprocessor::TemplateToken>
reporter::tokens::GetDatalessTokens() {
  struct utsname sysinfo;
  uname(&sysinfo);
  return std::vector<reporter::templateprocessor::TemplateToken>{
      CREATE_TOKEN_CAPTURE("%%HOSTNAME%%", sysinfo,
                           { return sysinfo.nodename; }),
      CREATE_TOKEN_CAPTURE("%%KERNEL_VERSION%%", sysinfo,
                           { return sysinfo.release; }),
      CREATE_TOKEN_CAPTURE("%%SYSTEM_NAME%%", sysinfo,
                           { return sysinfo.sysname; }),
      CREATE_TOKEN_CAPTURE("%%MACHINE_ARCH%%", sysinfo,
                           { return sysinfo.machine; }),
      CREATE_TOKEN_CAPTURE("%%DOMAIN_NAME%%", sysinfo,
                           { return sysinfo.domainname; })};
}
