#include "monitoringplugins/memoryplugin/memoryplugin.hpp"
#include <fstream>
#include <string>
#include "catch2/catch.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "imonitoringplugin/inputfilecontent.hpp"
#include "monitoringplugins/memoryplugin/memorypluginprocessors.hpp"

static const std::vector<imonitorplugin::PluginData> sample_data{
    imonitorplugin::PluginData{
        "MemoryPlugin", 1558784370, {{"mem_usage", 5}, {"swap_usage", 50}}},
    imonitorplugin::PluginData{
        "MemoryPlugin", 1558784375, {{"mem_usage", 5}, {"swap_usage", 50}}},
    imonitorplugin::PluginData{
        "MemoryPlugin", 1558784380, {{"mem_usage", 5}, {"swap_usage", 50}}}};

TEST_CASE("MemoryPlugin Data acquisition", "[MemoryPlugin]") {
  monitoringplugins::memoryplugin::MemoryPlugin plug{};
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
  dataprocessorhelper::gnuplot::GnuPlotBackend::instance().set_mock_call(true);
  auto processors = monitoringplugins::memoryplugin::CreateProcessors();
  auto time_series_processor = processors.front();

  auto time_series_response = time_series_processor->processor()(sample_data);

  REQUIRE(time_series_response == "bWVtX2NoYXJ0cy5ncAoK");
}
