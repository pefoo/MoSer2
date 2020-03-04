#include "monitoringplugins/loadplugin/loadplugin.hpp"
#include <fstream>
#include <string>
#include "catch2/catch.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "imonitoringplugin/inputfilecontent.hpp"
#include "monitoringplugins/loadplugin/loadpluginprocessors.hpp"
#include "utility/datastructure/table.hpp"

TEST_CASE("LoadPlugin Data acquisition", "[LoadPlugin]") {
  monitoringplugins::loadplugin::LoadPlugin plug{};
  plug.Init();
  auto data = plug.AcquireData({})[0];

  REQUIRE(data.data().size() == 3);

  REQUIRE(data.data().at(0).first == "m1");
  REQUIRE(data.data().at(1).first == "m5");
  REQUIRE(data.data().at(2).first == "m15");

  // Since actual data is acquired, checking exact values is not possible
  REQUIRE(std::any_cast<float>(data.data().at(0).second) > 0.0f);
  REQUIRE(std::any_cast<float>(data.data().at(1).second) > 0.0f);
  REQUIRE(std::any_cast<float>(data.data().at(2).second) > 0.0f);
}

TEST_CASE("LoadPlugin Data processor", "[LoadPlugin]") {
  utility::datastructure::Table sample_data{"LoadPlugin"};
  sample_data.AddColumn(
      utility::datastructure::DataColumn<int>{"m1", {5, 5, 5}});
  sample_data.AddColumn(
      utility::datastructure::DataColumn<int>{"m5", {10, 10, 10}});
  sample_data.AddColumn(
      utility::datastructure::DataColumn<int>{"m15", {50, 50, 50}});
  sample_data.AddColumn(utility::datastructure::DataColumn<int>{
      "timestamp", {1558784370, 1558784375, 1558784380}});
  dataprocessorhelper::gnuplot::GnuPlotBackend::instance().set_mock_call(true);
  auto processor = monitoringplugins::loadplugin::CreateProcessors().front();
  auto time_series_response = processor->processor()(sample_data);

  REQUIRE(time_series_response == "ImxvYWRfY2hhcnRzLmdwIgoK");
}
