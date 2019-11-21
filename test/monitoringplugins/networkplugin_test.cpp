#include "monitoringplugins/networkplugin/networkplugin.hpp"
#include <fstream>
#include <string>
#include "catch2/catch.hpp"
#include "configurationinjector.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "imonitoringplugin/inputfilecontent.hpp"
#include "monitoringplugins/networkplugin/constants.hpp"
#include "monitoringplugins/networkplugin/networkpluginprocessors.hpp"

static const std::vector<imonitorplugin::PluginData> sample_data{
    imonitorplugin::PluginData{
        "NetworkPlugin", 1558784370, {{"rx_bytes", 5}, {"tx_bytes", 10}}},
    imonitorplugin::PluginData{
        "NetworkPlugin", 1558784375, {{"rx_bytes", 5}, {"tx_bytes", 10}}},
    imonitorplugin::PluginData{
        "NetowrkPlugin", 1558784380, {{"rx_bytes", 5}, {"tx_bytes", 10}}}};

TEST_CASE("NetworkPlugin Data acquisition", "[NetworkPlugin]") {
  ConfigurationInjector c{
      monitoringplugins::networkplugin::constants::kPluginName};
  monitoringplugins::networkplugin::NetworkPlugin plug{};
  auto data = plug.AcquireData(
      {{"/sys/class/net/enp0s31f6/statistics/rx_bytes",
        imonitorplugin::InputFileContent{0, "50", "100", 1, 1}},
       {"/sys/class/net/enp0s31f6/statistics/tx_bytes",
        imonitorplugin::InputFileContent{0, "60", "130", 1, 1}}})[0];

  REQUIRE(data.data().size() == 2);
  REQUIRE(data.data().at(0).first == "rx");
  REQUIRE(data.data().at(1).first == "tx");

  REQUIRE(std::any_cast<int64_t>(data.data().at(0).second) == 50);
  REQUIRE(std::any_cast<int64_t>(data.data().at(1).second) == 70);
}

TEST_CASE("NetworkPlugin Data processor", "[NetworkPlugin]") {
  dataprocessorhelper::gnuplot::GnuPlotBackend::instance().set_mock_call(true);
  auto processors = monitoringplugins::networkplugin::CreateProcessors();
  auto time_series_processor = processors.front();

  auto time_series_response = time_series_processor->processor()(sample_data);
  REQUIRE(time_series_response == "bmV0d29ya3BsdWdpbl9jaGFydHMuZ3AKCg==");
}
