#ifndef MEMORYPLUGIN_H
#define MEMORYPLUGIN_H
// LCOV_EXCL_START
#include <string>
#include <unordered_map>
#include "monitoringpluginbase/monitorpluginbase.hpp"

namespace monitoringplugins {
namespace memoryplugin {
///
/// \brief The cpu plugin
///
class MemoryPlugin : public monitoringpluginbase::MonitorPluginBase {
 public:
  MemoryPlugin();

 protected:
  imonitorplugin::PluginData::data_vector AcquireDataInternal(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>
          &&input_file) const override;

 private:
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(MemoryPlugin)
CREATE_DEFAULT_DESTRUCTOR_FACTORY(MemoryPlugin)
// LCOV_EXCL_STOP
}  // namespace memoryplugin
}  // namespace monitoringplugins
#endif  //  MEMORYPLUGIN_H
