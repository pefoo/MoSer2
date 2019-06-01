#ifndef DISKPLUGIN_H
#define DISKPLUGIN_H

#include <string>
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
      imonitorplugin::InputFileContent &&input_file) const override;

 private:
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(DiskPlugin);
CREATE_DEFAULT_DESTRUCTOR_FACTORY(DiskPlugin);

}  // namespace diskplugin
}  // namespace monitoringplugins
#endif  // DISKPLUGIN_H
