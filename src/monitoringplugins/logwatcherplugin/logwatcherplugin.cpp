#include "monitoringplugins/logwatcherplugin/logwatcherplugin.hpp"
#include <filesystem>
#include <fstream>
#include <regex>
#include <utility>
#include "monitoringpluginbase/pluginconfigselector.hpp"
#include "monitoringplugins/logwatcherplugin/constants.hpp"
#include "utility/helper/stringhelper.hpp"

namespace monitoringplugins {
namespace logwatcherplugin {

using namespace logwatcherplugin::constants;
LogWatcherPlugin::LogWatcherPlugin()
    : monitoringpluginbase::MonitorPluginBase(constants::kPluginName) {}

void LogWatcherPlugin::Init() {
  if (!this->settings_) {
    this->ThrowPluginException(
        "The log watcher plugin requires a configuration.");
  }
  for (auto const& section : this->settings_->Sections()) {
    if (section == kSettingsSectionReporter) continue;
    this->files_.push_back(LogWatcherPlugin::LogFile(
        this->settings_->GetValue(kDisplayName, section),
        this->settings_->GetValue(kFileName, section),
        this->settings_->GetValue(kPattern, section),
        this->settings_->GetValue(kTags, section)));
  }
}

std::vector<std::string> LogWatcherPlugin::DoSanityCheck() const {
  std::vector<std::string> messages;
  for (auto const& log_file : this->files_) {
    if (!std::filesystem::exists(log_file.file_)) {
      messages.push_back("Log file : " + log_file.file_ + " was not found!");
      continue;
    }
  }
  return messages;
}

std::vector<std::shared_ptr<imonitorplugin::IPluginConfigSelector> >
LogWatcherPlugin::GetConfigSelectors(std::ostream& os, std::istream& is) const {
  return {
      // TODO Is this field actually required?
      std::shared_ptr<imonitorplugin::IPluginConfigSelector>(
          new monitoringpluginbase::PluginConfigSelector(
              constants::kDisplayName,
              [&os, &is]() {
                os << "Select a display name for the first log watcher rule."
                   << std::endl;
                os << "You may add multiple rules to the configuration file by "
                      "manually. This process will get you started with your "
                      "first rule. You may change the rule header (default: "
                      "Category1) to whatever you like (avoid whitespace)."
                   << std::endl;
                os << std::endl;
                os << ">>";
                std::string value;
                getline(is, value);
                return value;
              },
              "Category1")),
      std::shared_ptr<imonitorplugin::IPluginConfigSelector>(
          new monitoringpluginbase::PluginConfigSelector(
              constants::kFileName,
              [&os, &is]() {
                os << "Select a file to monitor." << std::endl;
                os << std::endl;
                os << ">>";
                std::string value;
                getline(is, value);
                return value;
              },
              "Category1")),
      std::shared_ptr<imonitorplugin::IPluginConfigSelector>(
          new monitoringpluginbase::PluginConfigSelector(
              constants::kPattern,
              [&os, &is]() {
                os << "Select a pattern to look for (regular expression)."
                   << std::endl;
                os << std::endl;
                os << ">>";
                std::string value;
                getline(is, value);
                return value;
              },
              "Category1")),
      std::shared_ptr<imonitorplugin::IPluginConfigSelector>(
          new monitoringpluginbase::PluginConfigSelector(
              constants::kTags,
              [&os, &is]() {
                os << "Select the tags to apply. Separate each tag with a "
                      "comma."
                   << std::endl;
                os << std::endl;
                os << ">>";
                std::string value;
                getline(is, value);
                return value;
              },
              "Category1")),
      std::shared_ptr<imonitorplugin::IPluginConfigSelector>(
          new monitoringpluginbase::PluginConfigSelector(
              constants::kReportTags,
              [&os, &is]() {
                os << "Select the tag names you want to include in your report."
                   << std::endl;
                os << std::endl;
                os << ">>";
                std::string value;
                getline(is, value);
                return value;
              },
              "Reporter"))};
}

std::vector<imonitorplugin::PluginData> LogWatcherPlugin::AcquireData(
    [[maybe_unused]] std::unordered_map<
        std::string, imonitorplugin::InputFileContent>&& input_file) {
  std::vector<imonitorplugin::PluginData> data;
  for (auto& log_file : this->files_) {
    // Initial run - set stream positions to end of file
    if (log_file.position_ == -1) {
      std::ifstream s{log_file.file_, std::ios_base::ate};
      log_file.position_ = s.tellg();
      continue;
    }

    std::ifstream s{log_file.file_, std::ios_base::ate};
    // File was larger last time. Most likely a rolling log file -> set position
    // to beginning of the file
    if (log_file.position_ > s.tellg()) {
      log_file.position_ = 0;
    }
    // No new content
    if (log_file.position_ == s.tellg()) {
      continue;
    }
    s.seekg(log_file.position_);
    std::string line;
    while (std::getline(s, line)) {
      if (!utility::helper::StringRgxGrep(line, log_file.pattern_)) {
        continue;
      }
      data.push_back(imonitorplugin::PluginData{
          this->name(),
          this->MakeTimestamp(),
          {{kDbFieldEntry, line}, {kDbFieldTags, log_file.tags_}}});
    }
    log_file.position_ = s.tellg();
  }
  return data;
}

imonitorplugin::PluginData::data_vector LogWatcherPlugin::AcquireDataInternal(
    [[maybe_unused]] std::unordered_map<
        std::string, imonitorplugin::InputFileContent>&& input_file) {
  ThrowPluginException(
      "This plugin is using its own AcquireData implementation.");
}

}  // namespace logwatcherplugin
}  // namespace monitoringplugins
