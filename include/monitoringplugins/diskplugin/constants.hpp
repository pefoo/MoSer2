#ifndef CPUPLUGINCONSTANTS_HPP
#define CPUPLUGINCONSTANTS_HPP

namespace monitoringplugins {
namespace diskplugin {
namespace constants {

static constexpr char kGpArgFileName[] = "file_name";
static constexpr char kGpArgTitle[] = "title";
static constexpr char kGpScriptName[] = "disk_charts.gp";
static constexpr char kPluginName[] = "DiskPlugin";
static constexpr char kDbFieldBytesRead[] = "bytes_read";
static constexpr char kDbFieldBytesWritten[] = "bytes_written";
static constexpr char kDbFieldUtilization[] = "utilization";
static constexpr char kDbFieldDriveLabel[] = "drive_label";

}  // namespace constants
}  // namespace diskplugin
}  // namespace monitoringplugins

#endif  // CONSTANTS_HPP
