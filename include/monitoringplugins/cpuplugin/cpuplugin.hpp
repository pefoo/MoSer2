#ifndef CPUPLUGIN_H
#define CPUPLUGIN_H

#include <string>
#include "monitoringpluginbase/monitorpluginbase.hpp"

namespace monitoringplugins {
namespace cpuplugin {
///
/// \brief The cpu plugin
///
class CpuPlugin : public monitoringpluginbase::MonitorPluginBase {
 public:
  CpuPlugin();

 protected:
  data AcquireDataInternal() const override;

 private:
  struct CpuStat;
  CpuStat GetCpuStat() const;
  u_int core_count_;
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(CpuPlugin);
CREATE_DEFAULT_DESTRUCTOR_FACTORY(CpuPlugin);

}  // namespace cpuplugin
}  // namespace monitoringplugins
#endif  // CPUPLUGIN_H
