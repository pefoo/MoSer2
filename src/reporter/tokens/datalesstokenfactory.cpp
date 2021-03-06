#include "datalesstokenfactory.hpp"
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <ctime>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>
#include "reporter/viewmodel/processinformationviewmodel.hpp"
#include "reporter/views/processinformationviews.hpp"

std::vector<reporter::templateprocessor::TemplateToken>
reporter::tokens::GetDatalessTokens() {
  struct utsname info {};
  uname(&info);
  auto pi_model =
      std::make_shared<reporter::viewmodel::ProcessInformationViewModel>();
  auto pi_views =
      std::make_shared<reporter::views::ProcessInformationViews>(pi_model);
  return std::vector<reporter::templateprocessor::TemplateToken>{
      CREATE_TOKEN("%%UPTIME%%",
                   {
                     struct sysinfo info;
                     if (sysinfo(&info) != 0) {
                       return "";
                     }
                     auto total_seconds = info.uptime;
                     auto days = total_seconds / 86400;
                     auto hours = (total_seconds % 86400) / 3600;
                     auto minutes = (total_seconds % 3600) / 60;
                     auto seconds = total_seconds % 60;
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
      CREATE_TOKEN_CAPTURE(
          "%%PROC_MEM_USAGE%%", pi_views,
          { return pi_views->GetTopMemoryUsingProcessesWidget(5); }),
      CREATE_TOKEN_CAPTURE(
          "%%PROC_DISK_WRITE%%", pi_views,
          { return pi_views->GetTopDiskWriteProcessWidget(5); }),
      CREATE_TOKEN_CAPTURE(
          "%%PROC_DISK_READ%%", pi_views,
          { return pi_views->GetTopDiskReadProcessWidget(5); }),
      CREATE_TOKEN_CAPTURE("%%HOSTNAME%%", info, { return info.nodename; }),
      CREATE_TOKEN_CAPTURE("%%KERNEL_VERSION%%", info,
                           { return info.release; }),
      CREATE_TOKEN_CAPTURE("%%SYSTEM_NAME%%", info, { return info.sysname; }),
      CREATE_TOKEN_CAPTURE("%%MACHINE_ARCH%%", info, { return info.machine; }),
      CREATE_TOKEN_CAPTURE("%%DOMAIN_NAME%%", info,
                           { return info.domainname; })};
}
