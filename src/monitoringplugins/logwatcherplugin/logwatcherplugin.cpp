#include "monitoringplugins/logwatcherplugin/logwatcherplugin.hpp"
#include <filesystem>
#include <fstream>
#include <regex>
#include <utility>
#include "monitoringplugins/logwatcherplugin/constants.hpp"

namespace monitoringplugins {
namespace logwatcherplugin {

constexpr char kDisplayName[] = "DisplayName";
constexpr char kPattern[] = "Pattern";
constexpr char kFileName[] = "File";
constexpr char kTags[] = "Tags";

struct LogWatcherPlugin::LogFile {
  LogFile(std::string name, std::string file, std::string pattern,
          std::string tags)
      : name_(std::move(name)),
        file_(std::move(file)),
        pattern_(std::move(pattern)),
        tags_(std::move(tags)),
        position_(-1) {}
  std::string name_;
  std::string file_;
  std::string pattern_;
  std::string tags_;
  long position_;
};

LogWatcherPlugin::LogWatcherPlugin()
    : monitoringpluginbase::MonitorPluginBase(constants::kPluginName) {
  if (!this->settings_) {
    this->ThrowPluginException(
        "The log watcher plugin requires a configuration.");
  }
  for (auto const& section : this->settings_->Sections()) {
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
      if (!std::regex_match(line, std::regex{log_file.pattern_})) {
        continue;
      }
      data.push_back(imonitorplugin::PluginData{
          this->name(),
          this->MakeTimestamp(),
          {{"entry", line}, {"tags", log_file.tags_}}});
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
