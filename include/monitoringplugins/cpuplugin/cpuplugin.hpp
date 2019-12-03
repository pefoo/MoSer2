#ifndef CPUPLUGIN_H
#define CPUPLUGIN_H
// LCOV_EXCL_START

#include <string>
#include <unordered_map>
#include <vector>
#include "monitoringpluginbase/monitorpluginbase.hpp"

namespace monitoringplugins {
namespace cpuplugin {
///
/// \brief The cpu plugin
///
class CpuPlugin : public monitoringpluginbase::MonitorPluginBase {
 public:
  CpuPlugin();

  [[nodiscard]] std::vector<std::string> DoSanityCheck() const override;

 protected:
  imonitorplugin::PluginData::data_vector AcquireDataInternal(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>
          &&input_file) override;

 private:
  struct CpuStat;
  [[nodiscard]] CpuStat GetCpuStat(const std::string &snapshot) const;
  u_int core_count_;
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(CpuPlugin)
CREATE_DEFAULT_DESTRUCTOR_FACTORY(CpuPlugin)

}  // namespace cpuplugin
}  // namespace monitoringplugins

// LCOV_EXCL_STOP
#endif  // CPUPLUGIN_H
