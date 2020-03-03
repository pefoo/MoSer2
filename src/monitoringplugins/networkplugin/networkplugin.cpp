#include "monitoringplugins/networkplugin/networkplugin.hpp"
#include <ifaddrs.h>
#include "monitoringplugins/networkplugin/constants.hpp"

namespace monitoringplugins {
namespace networkplugin {

NetworkPlugin::NetworkPlugin()
    : monitoringpluginbase::MonitorPluginBase(constants::kPluginName) {}

void NetworkPlugin::Init() {
  if (this->settings_) {
    auto interface = this->settings_->GetValue("Interface", "");
    if (interface.empty()) {
      ThrowPluginException(
          "No interface settings found. Add the Interface setting to the "
          "config file.");
    }
    this->rx_bytes_file =
        "/sys/class/net/" + interface + "/statistics/rx_bytes";
    this->tx_bytes_file =
        "/sys/class/net/" + interface + "/statistics/tx_bytes";
  } else {
    this->ThrowPluginException(
        "No plugin settings found. The interface must be configured!");
  }

  this->RegisterFileToRead(rx_bytes_file);
  this->RegisterFileToRead(tx_bytes_file);
}

std::vector<std::string> NetworkPlugin::DoSanityCheck() const {
  auto interface = this->settings_->GetValue("Interface", "");
  struct ifaddrs* ifaddr;
  getifaddrs(&ifaddr);
  for (auto ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
    if (interface == ifa->ifa_name) {
      return {};
    }
  }
  return {"The configured network interface " + interface + " was not found."};
}

imonitorplugin::PluginData::data_vector NetworkPlugin::AcquireDataInternal(
    std::unordered_map<std::string, imonitorplugin::InputFileContent>&&
        input_file) {
  int64_t rx1 = std::stol(input_file[this->rx_bytes_file].snapshot_1());
  int64_t rx2 = std::stol(input_file[this->rx_bytes_file].snapshot_2());
  int64_t tx1 = std::stol(input_file[this->tx_bytes_file].snapshot_1());
  int64_t tx2 = std::stol(input_file[this->tx_bytes_file].snapshot_2());

  return {{"rx", rx2 - rx1}, {"tx", tx2 - tx1}};
}

}  // namespace networkplugin
}  // namespace monitoringplugins
