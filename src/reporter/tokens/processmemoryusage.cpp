#include "reporter/tokens/processmemoryusage.hpp"
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
static constexpr char kTableColumnStart[] = "<td class=default>";
static constexpr char kTableColumnEnd[] = "</td>";

struct reporter::tokens::ProcessMemoryUsage::ProcessStatus {
  std::string name{""};
  std::string cmd_line{""};
  size_t rss_anon{0};
  size_t vm_size{0};
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
              return a.rss_anon > b.rss_anon;
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
    ss << kTableColumnStart << std::round(pow(2.0, -10.0) * entry.rss_anon)
       << kTableColumnEnd;
    ss << kTableColumnStart << entry.cmd_line << kTableColumnEnd;
    ss << kTableRowEnd;
  }
  ss << kTableEnd;
  return ss.str();
}

void reporter::tokens::ProcessMemoryUsage::ReadProcessStatus() {
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
        if (StringStartsWith(line, "RssAnon")) {
          StringRgxGrep(line, kFieldRssAnon, &sm);
          status.rss_anon = std::stoul(sm[1]);
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
      std::ifstream cmdline_stream{
          PathCombine({entry.path(), kFileProcCmdLine})};
      status.cmd_line =
          std::string((std::istreambuf_iterator<char>(cmdline_stream)),
                      std::istreambuf_iterator<char>());
      this->proc_status_.emplace_back(status);
    }
  }
}
