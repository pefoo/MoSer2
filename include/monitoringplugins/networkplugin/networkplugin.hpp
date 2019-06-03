#ifndef NETWORKPLUGIN_H
#define NETWORKPLUGIN_H

#include <string>
#include <unordered_map>
#include "monitoringpluginbase/monitorpluginbase.hpp"

namespace monitoringplugins {
namespace networkplugin {

///
/// \brief TODO documentation
///
class NetworkPlugin : public monitoringpluginbase::MonitorPluginBase {
 public:
  NetworkPlugin();

 protected:
  imonitorplugin::PluginData::data_vector AcquireDataInternal(
      std::unordered_map<std::string, imonitorplugin::InputFileContent>
          &&input_file) const override;

 private:
  std::string rx_bytes_file;
  std::string tx_bytes_file;
};

CREATE_DETAULT_CONSTRUCTOR_FACTORY(NetworkPlugin);
CREATE_DEFAULT_DESTRUCTOR_FACTORY(NetworkPlugin);

}  // namespace networkplugin
}  // namespace monitoringplugins

#endif  //
