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
  std::vector<std::string> DoSanityCheck() const override;

 protected:
  std::vector<imonitorplugin::PluginData> AcquireData(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>
          &&input_file) override;
  imonitorplugin::PluginData::data_vector AcquireDataInternal(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>
          &&input_file) override;

 private:
  struct LogFile;
  std::vector<LogFile> files_;
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(LogWatcherPlugin)
CREATE_DEFAULT_DESTRUCTOR_FACTORY(LogWatcherPlugin)

}  // namespace logwatcherplugin
}  // namespace monitoringplugins

#endif  //
