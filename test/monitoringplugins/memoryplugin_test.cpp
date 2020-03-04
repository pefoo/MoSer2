#include "monitoringplugins/memoryplugin/memoryplugin.hpp"
#include <fstream>
#include <string>
#include "catch2/catch.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "imonitoringplugin/inputfilecontent.hpp"
#include "monitoringplugins/memoryplugin/memorypluginprocessors.hpp"
#include "utility/datastructure/table.hpp"

TEST_CASE("MemoryPlugin Data acquisition", "[MemoryPlugin]") {
  monitoringplugins::memoryplugin::MemoryPlugin plug{};
  plug.Init();
  auto data = plug.AcquireData({})[0];

  REQUIRE(data.data().size() == 2);
  REQUIRE(data.data().at(0).first == "mem_usage");
  REQUIRE(data.data().at(1).first == "swap_usage");

  REQUIRE(std::any_cast<int>(data.data().at(0).second) > 0);
  REQUIRE(std::any_cast<int>(data.data().at(0).second) < 100);
  REQUIRE(std::any_cast<int>(data.data().at(1).second) >= 0);
  REQUIRE(std::any_cast<int>(data.data().at(1).second) < 100);
}

TEST_CASE("MemoryPlugin Data processor", "[MemoryPlugin]") {
  utility::datastructure::Table sample_data{"MemoryPlugin"};
  sample_data.AddColumn(
      utility::datastructure::DataColumn<int>{"mem_usage", {5, 5, 5}});
  sample_data.AddColumn(
      utility::datastructure::DataColumn<int>{"swap_usage", {50, 50, 50}});
  sample_data.AddColumn(utility::datastructure::DataColumn<int>{
      "timestamp", {1558784370, 1558784375, 1558784380}});

  dataprocessorhelper::gnuplot::GnuPlotBackend::instance().set_mock_call(true);
  auto processors = monitoringplugins::memoryplugin::CreateProcessors();
  auto time_series_processor = processors.front();

  auto time_series_response = time_series_processor->processor()(sample_data);

  REQUIRE(time_series_response == "Im1lbV9jaGFydHMuZ3AiCgo=");
}
