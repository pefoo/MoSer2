#ifndef MEMORYPLUGIN_H
#define MEMORYPLUGIN_H

#include <string>
#include "monitoringpluginbase/monitorpluginbase.hpp"

namespace monitoringplugins {
namespace memoryplugin{
///
/// \brief The cpu plugin
///
class MemoryPlugin : public monitoringpluginbase::MonitorPluginBase {
 public:
  MemoryPlugin();

 protected:
  imonitorplugin::PluginData::data_vector AcquireDataInternal(
      imonitorplugin::InputFileContent &&input_file) const override;

 private:
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(MemoryPlugin);
CREATE_DEFAULT_DESTRUCTOR_FACTORY(MemoryPlugin);

}  // namespace memoryplugin
}  // namespace monitoringplugins
#endif  //  MEMORYPLUGIN_H
