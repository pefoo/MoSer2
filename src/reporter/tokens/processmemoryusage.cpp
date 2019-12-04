#include "reporter/tokens/processmemoryusage.hpp"
#include <sys/utsname.h>
#include <unistd.h>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "utility/filesystem/fileaccesshelper.hpp"
#include "utility/helper/stringhelper.hpp"

using namespace utility::helper;
using namespace utility::filesystem;

// proc/{pid}/ related things
constexpr char kFieldRssAnon[] = R"(RssAnon:[\s\t]+(\d+).*)";
constexpr char kFieldVmSize[] = R"(VmSize:[\s\t]+(\d+).*)";
constexpr char kFieldPid[] = R"(Pid:[\s\t]+(\d+).*)";
constexpr char kFieldName[] = R"(Name:[\s\t]+(.*).*)";
constexpr char kFileProcStatus[] = "status";
constexpr char kFileProcCmdLine[] = "cmdline";

// html related things
static constexpr char kTableStart[] = "<table class=default>";
static constexpr char kTableEnd[] = "</table>";
static constexpr char kTableRowStart[] = "<tr class=default>";
static constexpr char kTableRowEnd[] = "</tr>";
static constexpr char kTableHeaderStart[] = "<th class=default>";
static constexpr char kTableHeaderEnd[] = "</th>";
static constexpr char kTableColumnStart[] =
    "<td class=default style='padding:4px'>";
static constexpr char kTableColumnEnd[] = "</td>";

///
/// \brief thr process status
///
struct reporter::tokens::ProcessMemoryUsage::ProcessStatus {
  // Process name
  std::string name{""};
  // Process command line
  std::string cmd_line{""};
  // Size of resident anonymous memory (this is considered the actual memory
  // usage of the process)
  size_t anonymous_memory{0};
  // Virtuel memory size of the process
  size_t vm_size{0};
  // Process id
  size_t pid{0};
};

reporter::tokens::ProcessMemoryUsage::ProcessMemoryUsage() {
  this->ReadProcessStatus();
}

// Required to make the forward declaration of ProcessStatus work ;)
reporter::tokens::ProcessMemoryUsage::~ProcessMemoryUsage() = default;

std::string reporter::tokens::ProcessMemoryUsage::GetMemoryUsageByProcessTable(
    uint entries) {
  std::sort(this->proc_status_.begin(), this->proc_status_.end(),
            [](const ProcessStatus& a, const ProcessStatus& b) {
              return a.anonymous_memory > b.anonymous_memory;
            });
  std::stringstream ss;
  ss << kTableStart << kTableRowStart;
  ss << kTableHeaderStart << "Process" << kTableHeaderEnd;
  ss << kTableHeaderStart << "Memory [MB]" << kTableHeaderEnd;
  ss << kTableHeaderStart << "Command line" << kTableHeaderEnd;
  ss << kTableRowEnd;

  for (size_t i = 0; i < entries && i < this->proc_status_.size(); ++i) {
    const auto& entry = this->proc_status_.at(i);
    ss << kTableRowStart;
    ss << kTableColumnStart << entry.name << kTableColumnEnd;
    ss << kTableColumnStart
       << std::round(pow(2.0, -10.0) * entry.anonymous_memory);
    ss << kTableColumnStart << entry.cmd_line << kTableColumnEnd;
    ss << kTableRowEnd;
  }
  ss << kTableEnd;
  return ss.str();
}

void reporter::tokens::ProcessMemoryUsage::ReadProcessStatus() {
  struct utsname unameData;
  uname(&unameData);
  std::istringstream stream{unameData.release};
  std::vector<std::string> version;
  std::string token;
  while (std::getline(stream, token, '.')) {
    version.push_back(token);
  }
  bool rss_anon_available =
      std::stoi(version[0]) >= 4 && std::stoi(version[1]) >= 5;
  for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
    if (entry.is_directory() && StringRgxGrep(entry.path(), R"(^/proc/\d+$)")) {
      auto l = PathCombine({entry.path(), "status"});
      std::ifstream status_stream{PathCombine({entry.path(), kFileProcStatus})};
      std::string line;
      ProcessStatus status;
      // It is way faster to check the beginning of the line FIRST and then use
      // regular expressions to retrieve the value (factor ~50).
      while (std::getline(status_stream, line)) {
        std::smatch sm;
        if (rss_anon_available && StringStartsWith(line, "RssAnon")) {
          StringRgxGrep(line, kFieldRssAnon, &sm);
          status.anonymous_memory = std::stoul(sm[1]);
          continue;
        }
        if (StringStartsWith(line, "VmSize")) {
          StringRgxGrep(line, kFieldVmSize, &sm);
          status.vm_size = std::stoul(sm[1]);
          continue;
        }
        if (StringStartsWith(line, "Name")) {
          StringRgxGrep(line, kFieldName, &sm);
          status.name = sm[1];
          continue;
        }
        if (StringStartsWith(line, "Pid")) {
          StringRgxGrep(line, kFieldPid, &sm);
          status.pid = std::stoul(sm[1]);
          continue;
        }
      }
      if (!rss_anon_available) {
        // the field RssAnon was added in kernel version 4.5
        // use statm to get the resident set size and shared resident set size
        // (in pages)
        std::ifstream statm_stream{PathCombine({entry.path(), "statm"})};
        std::string statm((std::istreambuf_iterator<char>(statm_stream)),
                          std::istreambuf_iterator<char>());
        std::smatch sm;
        if (StringRgxGrep(statm, R"((\d+)\s+(\d+)\s+(\d+).*\n?)", &sm)) {
          size_t resident = std::stoul(sm[2]);
          size_t shared = std::stoul(sm[3]);
          long page_size = sysconf(_SC_PAGESIZE);
          // resident and shared are measured in pages. Use page size (converted
          // to KB) to get the memory usage in KB
          status.anonymous_memory =
              static_cast<size_t>((resident - shared) * (page_size / 1024.0));
        }
      }
      std::ifstream cmdline_stream{
          PathCombine({entry.path(), kFileProcCmdLine})};
      status.cmd_line =
          std::string((std::istreambuf_iterator<char>(cmdline_stream)),
                      std::istreambuf_iterator<char>());
      this->proc_status_.emplace_back(status);
    }
  }
}
