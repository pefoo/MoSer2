#include "datalesstokenfactory.hpp"
#include <sys/utsname.h>

#define CREATETOKEN(token_name, body)               \
  ::reporter::templateprocessor::TemplateToken {    \
    token_name, [sysinfo]() -> std::string { body } \
  }

std::vector<reporter::templateprocessor::TemplateToken>
reporter::tokens::GetDatalessTokens() {
  struct utsname sysinfo;
  uname(&sysinfo);
  return std::vector<reporter::templateprocessor::TemplateToken>{
      CREATETOKEN("%%HOSTNAME%%", { return sysinfo.nodename; }),
      CREATETOKEN("%%KERNEL_VERSION%%", { return sysinfo.release; }),
      CREATETOKEN("%%SYSTEM_NAME%%", { return sysinfo.sysname; }),
      CREATETOKEN("%%MACHINE_ARCH%%", { return sysinfo.machine; }),
      CREATETOKEN("%%DOMAIN_NAME%%", { return sysinfo.domainname; })};
}
