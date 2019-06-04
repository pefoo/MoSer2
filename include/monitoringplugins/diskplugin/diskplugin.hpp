#ifndef DISKPLUGIN_H
#define DISKPLUGIN_H

#include <string>
#include <unordered_map>
#include "monitoringpluginbase/monitorpluginbase.hpp"
#include <vector>

namespace monitoringplugins {
namespace diskplugin {
///
/// \brief The diskplugin
///
class DiskPlugin : public monitoringpluginbase::MonitorPluginBase {
 public:
  DiskPlugin();

 protected:
  imonitorplugin::PluginData::data_vector AcquireDataInternal(
      std::unordered_map<std::string,
      imonitorplugin::InputFileContent>&& input_file) const override;

 private:
  struct DiskStat;
  std::unordered_map<std::string, DiskStat> ParseDiskstat(
      std::vector<std::string> devices, const std::string& diskstat) const;
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(DiskPlugin)
CREATE_DEFAULT_DESTRUCTOR_FACTORY(DiskPlugin)

}  // namespace diskplugin
}  // namespace monitoringplugins
#endif  // DISKPLUGIN_H
