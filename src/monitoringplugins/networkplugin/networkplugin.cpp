#include "monitoringplugins/networkplugin/networkplugin.hpp"
#include "monitoringplugins/networkplugin/constants.hpp"

namespace monitoringplugins {
namespace networkplugin {

NetworkPlugin::NetworkPlugin()
    : monitoringpluginbase::MonitorPluginBase(constants::kPluginName) {}

imonitorplugin::PluginData::data_vector NetworkPlugin::AcquireDataInternal(
    std::unordered_map<std::string, imonitorplugin::InputFileContent>
        &&input_file) const {}

}  // namespace networkplugin
}  // namespace monitoringplugins
