#ifndef LOGWATCHERPLUGIN_H
#define LOGWATCHERPLUGIN_H

#include <string>
#include <unordered_map>
#include "monitoringpluginbase/monitorpluginbase.hpp"

namespace monitoringplugins {
namespace logwatcherplugin {

///
/// \brief THe log watcher plugin
///
class LogWatcherPlugin : public monitoringpluginbase::MonitorPluginBase {
 public:
  LogWatcherPlugin();
  [[nodiscard]] std::vector<std::string> DoSanityCheck() const override;
  std::vector<imonitorplugin::PluginData> AcquireData(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>
          &&input_file) override;

 protected:
  imonitorplugin::PluginData::data_vector AcquireDataInternal(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>
          &&input_file) override;

 private:
  struct LogFile {
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

  std::vector<LogFile> files_;
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(LogWatcherPlugin)
CREATE_DEFAULT_DESTRUCTOR_FACTORY(LogWatcherPlugin)

}  // namespace logwatcherplugin
}  // namespace monitoringplugins

#endif  //
