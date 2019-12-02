#include "monitoringplugins/networkplugin/networkplugin.hpp"
#include <fstream>
#include <string>
#include "catch2/catch.hpp"
#include "configurationinjector.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "imonitoringplugin/inputfilecontent.hpp"
#include "monitoringplugins/networkplugin/constants.hpp"
#include "monitoringplugins/networkplugin/networkpluginprocessors.hpp"
#include "utility/datastructure/table.hpp"

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
  utility::datastructure::Table sample_data{"NetworkPlugin"};
  sample_data.AddColumn(
      utility::datastructure::DataColumn<int>{"rx_bytes", {5, 5, 5}});
  sample_data.AddColumn(
      utility::datastructure::DataColumn<int>{"tx_bytes", {10, 10, 10}});
  sample_data.AddColumn(utility::datastructure::DataColumn<int>{
      "timestamp", {1558784370, 1558784375, 1558784380}});

  dataprocessorhelper::gnuplot::GnuPlotBackend::instance().set_mock_call(true);
  auto processors = monitoringplugins::networkplugin::CreateProcessors();
  auto time_series_processor = processors.front();

  auto time_series_response = time_series_processor->processor()(sample_data);
  REQUIRE(time_series_response == "bmV0d29ya3BsdWdpbl9jaGFydHMuZ3AKCg==");
}
