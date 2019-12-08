#include "reporter/datamodel/processinformation.hpp"
#include <sys/utsname.h>
#include <unistd.h>
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
constexpr char kFieldName[] = R"(Name:[\s\t]+(.*).*)";
constexpr char kFieldIoWrite[] = R"(write_bytes:[\s\t]+(\d+))";
constexpr char kFieldIoRead[] = R"(read_bytes:[\s\t]+(\d+))";
constexpr char kFileProcStatus[] = "status";
constexpr char kFileProcCmdLine[] = "cmdline";
constexpr char kFileIO[] = "io";

reporter::datamodel::ProcessInformation::ProcessInformation(size_t pid)
    : pid_(pid) {
  this->Refresh();
}

void reporter::datamodel::ProcessInformation::Refresh() {
  auto t = PathCombine({"/proc", std::to_string(this->pid())});
  if (!std::filesystem::exists(
          PathCombine({"/proc", std::to_string(this->pid())}))) {
    throw std::runtime_error("A process with the pid " +
                             std::to_string(this->pid()) + " does not exist!");
  }
  // quite unlike the kernel version changes during runtime, thus the check is
  // made once
  static bool kernel_version_check_done{false};
  static bool rss_anon_available{false};
  if (!kernel_version_check_done) {
    // get kernel version
    struct utsname unameData;
    uname(&unameData);
    std::istringstream stream{unameData.release};
    std::vector<std::string> version;
    std::string token;
    while (std::getline(stream, token, '.')) {
      version.push_back(token);
    }
    rss_anon_available =
        std::stoi(version[0]) >= 4 && std::stoi(version[1]) >= 5;
    kernel_version_check_done = true;
  }

  std::ifstream status_stream{
      PathCombine({"/proc", std::to_string(this->pid()), kFileProcStatus})};
  std::string line;
  while (std::getline(status_stream, line)) {
    std::smatch sm;
    if (rss_anon_available && StringStartsWith(line, "RssAnon")) {
      StringRgxGrep(line, kFieldRssAnon, &sm);
      this->anonymous_memory_ = std::stoul(sm[1]);
      continue;
    }
    if (StringStartsWith(line, "VmSize")) {
      StringRgxGrep(line, kFieldVmSize, &sm);
      this->virtual_memory_ = std::stoul(sm[1]);
      continue;
    }
    if (StringStartsWith(line, "Name")) {
      StringRgxGrep(line, kFieldName, &sm);
      this->name_ = sm[1];
      continue;
    }
  }
  if (!rss_anon_available) {
    // the field RssAnon was added in kernel version 4.5
    // use statm to get the resident set size and shared resident set size
    // (in pages)
    std::ifstream statm_stream{
        PathCombine({"/proc", std::to_string(this->pid()), "statm"})};
    std::string statm((std::istreambuf_iterator<char>(statm_stream)),
                      std::istreambuf_iterator<char>());
    std::smatch sm;
    if (StringRgxGrep(statm, R"((\d+)\s+(\d+)\s+(\d+).*\n?)", &sm)) {
      size_t resident = std::stoul(sm[2]);
      size_t shared = std::stoul(sm[3]);
      long page_size = sysconf(_SC_PAGESIZE);
      // resident and shared are measured in pages. Use page size (converted
      // to KB) to get the memory usage in KB
      this->anonymous_memory_ =
          static_cast<size_t>((resident - shared) * (page_size / 1024.0));
    }
  }
  std::ifstream cmdline_stream{
      PathCombine({"/proc", std::to_string(this->pid()), kFileProcCmdLine})};
  this->cmd_line_ =
      std::string((std::istreambuf_iterator<char>(cmdline_stream)),
                  std::istreambuf_iterator<char>());
  // The value is littered with null terminators. get rid of em!
  size_t pos = this->cmd_line_.find('\0');
  while (pos != std::string::npos) {
    this->cmd_line_.replace(pos, 1, " ");
    pos = this->cmd_line_.find('\0', pos + 1);
  }

  auto io_file = PathCombine({"/proc", std::to_string(this->pid()), kFileIO});
  // Access to this file might be denied
  if (access(io_file.c_str(), R_OK) == 0) {
    std::ifstream io_stream{io_file};
    std::string line;
    std::smatch sm;
    while (std::getline(io_stream, line)) {
      if (StringStartsWith(line, "read_bytes")) {
        StringRgxGrep(line, kFieldIoRead, &sm);
        this->disk_read_ = std::stoul(sm[1]);
        continue;
      }
      if (StringStartsWith(line, "write_bytes")) {
        StringRgxGrep(line, kFieldIoWrite, &sm);
        this->disk_write_ = std::stoul(sm[1]);
        continue;
      }
    }
  }
}

std::string reporter::datamodel::ProcessInformation::name() const {
  return this->name_;
}

std::string reporter::datamodel::ProcessInformation::cmd_line() const {
  return this->cmd_line_;
}

size_t reporter::datamodel::ProcessInformation::anonymous_memory() const {
  return this->anonymous_memory_;
}

size_t reporter::datamodel::ProcessInformation::virtual_memory() const {
  return this->virtual_memory_;
}

size_t reporter::datamodel::ProcessInformation::pid() const {
  return this->pid_;
}

size_t reporter::datamodel::ProcessInformation::disk_read() const {
  return this->disk_read_;
}

size_t reporter::datamodel::ProcessInformation::disk_write() const {
  return this->disk_write_;
}

bool reporter::datamodel::ProcessInformation::operator<(
    const reporter::datamodel::ProcessInformation &o) const {
  return this->name() == o.name() ? this->cmd_line() < o.cmd_line()
                                  : this->name() < o.name();
}
