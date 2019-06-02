#include "monitoringplugins/diskplugin/diskplugin.hpp"
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <regex>
#include <string>
#include <thread>
#include <utility>
#include <vector>
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
    : monitoringpluginbase::MonitorPluginBase(constants::kPluginName) {
  this->RegisterFileToRead("/proc/diskstats");
}

imonitorplugin::PluginData::data_vector
monitoringplugins::diskplugin::DiskPlugin::AcquireDataInternal(
    imonitorplugin::InputFileContent &&input_file) const {
  auto devices = std::stringstream{this->settings_->GetValue("Devices", "")};
  std::string tmp;
  std::vector<std::string> device_list;
  while (std::getline(devices, tmp, ';')) {
    device_list.push_back(tmp);
  }
  auto stats_1 = this->ParseDiskstat(device_list, input_file.snapshot_1());
  auto stats_2 = this->ParseDiskstat(device_list, input_file.snapshot_2());
  int64_t sector_size = std::stol(this->settings_->GetValue("SectorSize", ""));
  imonitorplugin::PluginData::data_vector data;

  for (const auto &device_name : device_list) {
    auto t_io_total =
        stats_2.at(device_name).t_io - stats_1.at(device_name).t_io;
    auto utilization = (static_cast<double>(t_io_total) /
                        (input_file.timestamp_2() - input_file.timestamp_1())) *
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

std::unordered_map<std::string,
                   monitoringplugins::diskplugin::DiskPlugin::DiskStat>
monitoringplugins::diskplugin::DiskPlugin::ParseDiskstat(
    std::vector<std::string> devices, const std::string &diskstat) const {
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
    if (std::find(devices.begin(), devices.end(), d) != devices.end()) {
      stats[d] = DiskStat(d, std::stoul(match[6]), std::stoul(match[10]),
                          std::stoul(match[13]));
    }
  }
  return stats;
}
