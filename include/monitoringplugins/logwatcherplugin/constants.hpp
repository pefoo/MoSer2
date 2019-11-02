#ifndef LOGWATCHERPLUGINCONSTANTS_H
#define LOGWATCHERPLUGINCONSTANTS_H

namespace monitoringplugins {
namespace logwatcherplugin {
namespace constants {

static constexpr char kPluginName[] = "LogWatcherPlugin";
static constexpr char kGpScriptName[] = "logwatcherplugin_charts.gp";
static constexpr char kGpArgFileName[] = "file_name";
static constexpr char kSettingsSectionReporter[] = "Reporter";
static constexpr char kReportTags[] = "ReportTags";
static constexpr char kDisplayName[] = "DisplayName";
static constexpr char kPattern[] = "Pattern";
static constexpr char kFileName[] = "File";
static constexpr char kTags[] = "Tags";
static constexpr char kDbFieldEntry[] = "entry";
static constexpr char kDbFieldTags[] = "tags";

}  // namespace constants
}  // namespace logwatcherplugin
}  // namespace monitoringplugins

#endif  // LOGWATCHERPLUGINCONSTANTS_H
