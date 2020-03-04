#include "monitoringplugins/diskplugin/diskplugin.hpp"
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <regex>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include "monitoringpluginbase/pluginconfigselector.hpp"
#include "monitoringplugins/diskplugin/constants.hpp"

///
/// \brief The monitoringplugins::diskplugin::DiskPlugin::DiskStat struct
/// \details See
/// https://www.kernel.org/doc/Documentation/ABI/testing/procfs-diskstats
///
struct monitoringplugins::diskplugin::DiskPlugin::DiskStat {
  DiskStat() = default;
  DiskStat(std::string device, uint64_t sectors_read, uint64_t sectors_written,
           uint64_t t_io)
      : device(std::move(device)),
        sectors_read(sectors_read),
        sectors_written(sectors_written),
        t_io(t_io) {}

  std::string device;
  uint64_t sectors_read{};
  uint64_t sectors_written{};
  uint64_t t_io{};
};

monitoringplugins::diskplugin::DiskPlugin::DiskPlugin()
    : monitoringpluginbase::MonitorPluginBase(constants::kPluginName) {}

void monitoringplugins::diskplugin::DiskPlugin::Init() {
  this->RegisterFileToRead("/proc/diskstats");
  auto devices = std::stringstream{this->settings_->GetValue("Devices", "")};
  auto sectors =
      std::stringstream{this->settings_->GetValue("SectorSizes", "")};
  std::vector<int> sector_sizes;
  std::string tmp;
  while (std::getline(sectors, tmp, ';')) {
    sector_sizes.push_back(std::stoi(tmp));
  }
  tmp = "";
  size_t i = 0;
  while (std::getline(devices, tmp, ';')) {
    this->device_list_[tmp] = sector_sizes.at(i);
    ++i;
  }
}

std::vector<std::string>
monitoringplugins::diskplugin::DiskPlugin::DoSanityCheck() const {
  if (this->device_list_.empty()) {
    return {"Device list is empty. No drive to monitor."};
  }

  std::ifstream mounts{"/proc/mounts"};
  if (mounts.fail()) {
    return {
        "Failed to get the list of mounted devices. Failed to read "
        "/proc/mounts."};
  }
  std::vector<std::string> available_mounts;
  std::vector<std::string> messages;
  while (!mounts.eof()) {
    std::string tmp;
    mounts >> tmp;
    available_mounts.push_back(basename(tmp.c_str()));
    mounts.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  for (const auto &[device_name, sector_size] : this->device_list_) {
    // check if the configured device is actually available
    if (std::find(available_mounts.begin(), available_mounts.end(),
                  device_name) == available_mounts.end()) {
      messages.push_back("Failed to find device " + device_name);
    }

    // check if sector size is either 512 or 4096
    if (!(sector_size == 512 || sector_size == 4096)) {
      messages.push_back("The sector size for device " + device_name +
                         " is incorrect (" + std::to_string(sector_size) + ")");
    }
  }

  return messages;
}

imonitorplugin::PluginData::data_vector
monitoringplugins::diskplugin::DiskPlugin::AcquireDataInternal(
    std::unordered_map<std::string, imonitorplugin::InputFileContent>
        &&input_file) {
  auto stats_1 =
      this->ParseDiskstat(input_file["/proc/diskstats"].snapshot_1());
  auto stats_2 =
      this->ParseDiskstat(input_file["/proc/diskstats"].snapshot_2());

  imonitorplugin::PluginData::data_vector data;

  for (const auto &[device_name, sector_size] : this->device_list_) {
    auto t_io_total =
        stats_2.at(device_name).t_io - stats_1.at(device_name).t_io;
    auto utilization = (static_cast<double>(t_io_total) /
                        (input_file["/proc/diskstats"].timestamp_2() -
                         input_file["/proc/diskstats"].timestamp_1())) *
                       0.1;
    int64_t bytes_read = int64_t(stats_2.at(device_name).sectors_read -
                                 stats_1.at(device_name).sectors_read) *
                         sector_size;
    int64_t bytes_written = int64_t(stats_2.at(device_name).sectors_written -
                                    stats_1.at(device_name).sectors_written) *
                            sector_size;
    data.push_back({device_name + "_bytes_read", bytes_read});
    data.push_back({device_name + "_bytes_written", bytes_written});
    data.push_back({device_name + "_utilization", utilization});
  }

  return data;
}

std::vector<std::shared_ptr<imonitorplugin::IPluginConfigSelector> >
monitoringplugins::diskplugin::DiskPlugin::GetConfigSelectors(
    std::ostream &os, std::istream &is) const {
  return {// Selector for devices property
          std::shared_ptr<imonitorplugin::IPluginConfigSelector>(
              new monitoringpluginbase::PluginConfigSelector(
                  "Devices",
                  [&os, &is]() {
                    std::ifstream mounts{"/proc/mounts"};
                    if (mounts.fail()) {
                    }
                    std::stringstream ss{};
                    ss << "Select the devices you want to monitor. Separate "
                          "devices "
                          "using "
                          "a "
                          "semicolon."
                       << std::endl;
                    ss << "E.g.: /dev/sda1;/dev/sda4" << std::endl;
                    ss << std::endl;

                    auto ignored_types = {
                        "squashfs",   "cgroup",   "sysfs",      "tmpfs",
                        "proc",       "devtmpfs", "securityfs", "debugfs",
                        "rpc_pipefs", "cgroup2",  "devpts",     "binfmt_misc",
                        "tracefs",    "configfs", "fusectl",    "pstore",
                        "efivarfs",   "autofs",   "mqueue",     "hugetlbfs"};
                    while (!mounts.eof()) {
                      std::string device, mnt, type;
                      mounts >> device >> mnt >> type;
                      mounts.ignore(std::numeric_limits<std::streamsize>::max(),
                                    '\n');
                      if (std::find(ignored_types.begin(), ignored_types.end(),
                                    type) != ignored_types.end())
                        continue;
                      if (device.empty()) continue;

                      ss << device << " (Mount point: " << mnt
                         << " type: " << type << ")" << std::endl;
                    }
                    os << ss.str();
                    os << ">>";
                    std::string value;
                    getline(is, value);
                    return value;
                  })),
          // Selector for the SectorSizes property
          std::shared_ptr<imonitorplugin::IPluginConfigSelector>(
              new monitoringpluginbase::PluginConfigSelector(
                  "SectorSizes", [&os, &is]() {
                    os << "Select the sector sizes for the configured devices"
                       << std::endl;
                    os << "Select one sector size per configured device, "
                          "separate them using semicolons."
                       << std::endl;
                    os << "E.g.: 512;512" << std::endl;
                    os << "You may check the sector size using the following "
                          "command: sudo fdisk -l"
                       << std::endl;
                    os << std::endl;
                    os << ">>";
                    std::string value;
                    getline(is, value);
                    return value;
                  }))};
}

std::unordered_map<std::string,
                   monitoringplugins::diskplugin::DiskPlugin::DiskStat>
monitoringplugins::diskplugin::DiskPlugin::ParseDiskstat(
    const std::string &diskstat) const {
  std::stringstream s{diskstat};
  std::regex rgx(
      R"((\d+)\s+(\d+)\s+([\d\w]+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+))");
  std::smatch match;
  std::string line;
  std::unordered_map<std::string, DiskStat> stats;

  while (std::getline(s, line)) {
    if (!std::regex_search(line, match, rgx)) {
      continue;
    }
    std::string d = std::string(match[3]);
    if (this->device_list_.count(d)) {
      stats[d] = DiskStat(d, std::stoul(match[6]), std::stoul(match[10]),
                          std::stoul(match[13]));
    }
  }
  return stats;
}
