#include "monitoringplugins/diskplugin/diskpluginprocessors.hpp"
#include <sys/statvfs.h>
#include <cmath>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "dataprocessorhelper/settingshelper.hpp"
#include "monitoringplugins/diskplugin/constants.hpp"
#include "settingsprovider/isettingsprovider.hpp"
#include "utility/datastructure/table.hpp"

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
    auto devices = std::stringstream{settings->GetValue("Devices", "")};
    std::string device;
    std::vector<std::string> device_list;
    while (std::getline(devices, device, ';')) {
      device_list.push_back(device);
    }
    for (const auto& device : device_list) {
      processors.push_back(std::make_shared<
                           monitoringpluginbase::PluginDataProcessor>(
          TokenDiskTimeSeriesData(device),
          [device](utility::datastructure::Table data) -> std::string {
            if (data.MaxSize() == 0) {
              return "";
            }
            auto field_filter = [device](const std::string& c,
                                         const std::string& v) {
              if (c !=
                  monitoringplugins::diskplugin::constants::kDbFieldDriveLabel)
                return false;

              return v != device;
            };
            dataprocessorhelper::gnuplot::GnuPlotParameterDict params{};
            params.AddParameter(
                monitoringplugins::diskplugin::constants::kGpArgTitle, device,
                true);

            return dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
                monitoringplugins::diskplugin::constants::kGpScriptName, data,
                params, utility::datastructure::NoColumnFilter, field_filter);
          }));
    }
    processors.push_back(
        std::make_shared<monitoringpluginbase::PluginDataProcessor>(
            kTokenDiskUsage,
            [device_list]([[gnu::unused]] utility::datastructure::Table data)
                -> std::string {
              auto mount_points = GetMountPoints();
              std::stringstream out{};
              out << "<table style=\"width:100%\">";
              out << "<col width=50%/>";
              out << "<col width=50%/>";
              for (const auto& device : device_list) {
                if (mount_points.count("/dev/" + device) != 0) {
                  auto total =
                      GetDiskTotal(mount_points["/dev/" + device]) / pow(2, 30);
                  auto available =
                      GetDiskAvailable(mount_points["/dev/" + device]) /
                      pow(2, 30);
                  auto usage = (1 - available / total) * 100;

                  out << "<tr><td>" << device << "</td><td>"
                      << mount_points["/dev/" + device]
                      << "</td></tr><tr><td colspan='2'><div "
                         "style=\"border:1px solid black; "
                         "height:1.3em\"><div style=\"position:absolute; "
                         "display:table; width:99%; text-align:center \">"
                      << round(total - available) << "/" << round(total)
                      << " [Gb]</div> <div style=\"background-color:"
                      << (usage > 70 ? (usage > 90 ? "red" : "yellow")
                                     : "green")
                      << "; height:100%; width:" << usage
                      << "%\"></div></div></td></tr>";
                }
              }
              out << "</table>";
              return out.str();
            }));
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
