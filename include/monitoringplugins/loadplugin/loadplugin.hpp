#ifndef LOADPLUGIN_H
#define LOADPLUGIN_H
// LCOV_EXCL_START
#include <unordered_map>
#include "monitoringpluginbase/monitorpluginbase.hpp"

namespace monitoringplugins {
namespace loadplugin {
///
/// \brief The load average plugin
///
class LoadPlugin : public monitoringpluginbase::MonitorPluginBase {
 public:
  LoadPlugin();

 protected:
  imonitorplugin::PluginData::data_vector AcquireDataInternal(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>&&
          input_file) override;

 private:
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(LoadPlugin)
CREATE_DEFAULT_DESTRUCTOR_FACTORY(LoadPlugin)

}  // namespace loadplugin
}  // namespace monitoringplugins
// LCOV_EXCL_STOP
#endif  // LOADPLUGIN_H
