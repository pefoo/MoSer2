#ifndef DISKPLUGIN_H
#define DISKPLUGIN_H
// LCOV_EXCL_START

#include <string>
#include <unordered_map>
#include <vector>
#include "monitoringpluginbase/monitorpluginbase.hpp"

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
      std::unordered_map<std::string, imonitorplugin::InputFileContent>&&
          input_file) const override;

 private:
  struct DiskStat;
  std::unordered_map<std::string, DiskStat> ParseDiskstat(
      std::vector<std::string> devices, const std::string& diskstat) const;
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(DiskPlugin)
CREATE_DEFAULT_DESTRUCTOR_FACTORY(DiskPlugin)

}  // namespace diskplugin
}  // namespace monitoringplugins
// LCOV_EXCL_STOP
#endif  // DISKPLUGIN_H
