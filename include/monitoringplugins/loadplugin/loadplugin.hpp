#ifndef LOADPLUGIN_H
#define LOADPLUGIN_H

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
          input_file) const override;

 private:
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(LoadPlugin);
CREATE_DEFAULT_DESTRUCTOR_FACTORY(LoadPlugin);

}  // namespace loadplugin
}  // namespace monitoringplugins

#endif  // LOADPLUGIN_H
