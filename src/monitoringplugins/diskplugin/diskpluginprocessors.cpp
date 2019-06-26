#include "monitoringplugins/diskplugin/diskpluginprocessors.hpp"
#include <sys/statvfs.h>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "dataprocessorhelper/settingshelper.hpp"
#include "monitoringplugins/diskplugin/constants.hpp"
#include "settingsprovider/isettingsprovider.hpp"

#if __x86_64 || __ppc64__
using Statvfs = struct statvfs64;
const auto GetStatvfs = statvfs64;
#else
using Statvfs = struct statvfs;
const auto GetStatvfs = statvfs;
#endif

std::unordered_map<std::string, std::string> GetMountPoints();
uint64_t GetDiskAvailable(const std::string&);
uint64_t GetDiskTotal(const std::string&);

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringplugins::diskplugin::CreateProcessors() {
  auto settings = dataprocessorhelper::GetPluginSettings("DiskPlugin");
  std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>
      processors{};

  if (settings) {
    auto mount_points = GetMountPoints();
    auto devices = std::stringstream{settings->GetValue("Devices", "")};
    std::string device;
    std::vector<std::string> device_list;
    while (std::getline(devices, device, ';')) {
      processors.push_back(
          std::make_shared<monitoringpluginbase::PluginDataProcessor>(
              "%%DISK_" + device + "_TIME_SERIES_DATA%%",
              [device](std::vector<imonitorplugin::PluginData> records)
                  -> std::string {
                if (records.empty()) {
                  return "";
                }
                auto record_filter = [device](const std::string& key) {
                  return key.find(device) == 0;
                };
                dataprocessorhelper::gnuplot::GnuPlotParameterDict params{};

                return dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
                    monitoringplugins::diskplugin::constants::kGpScriptName,
                    records,
                    monitoringplugins::diskplugin::constants::kGpArgFileName,
                    params, ";", record_filter);
              }));
      if (mount_points.count("/dev/" + device) != 0) {
        auto u = GetDiskTotal(mount_points["/dev/" + device]);
        int i = 4;
      }
    }
  }

  return processors;
}

uint64_t GetDiskAvailable(const std::string& device) {
  Statvfs stat;
  GetStatvfs(device.c_str(), &stat);
  return stat.f_bsize * stat.f_bavail;
}

uint64_t GetDiskTotal(const std::string& device) {
  Statvfs stat;
  GetStatvfs(device.c_str(), &stat);
  return stat.f_frsize * stat.f_blocks;
}

std::unordered_map<std::string, std::string> GetMountPoints() {
  std::ifstream mounts{"/proc/mounts"};
  if (mounts.fail()) {
    return {};
  }
  std::unordered_map<std::string, std::string> points;
  while (!mounts.eof()) {
    std::string device;
    std::string point;
    mounts >> device >> point;
    points[device] = point;
    mounts.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return points;
}
