#include "monitoringplugins/loadplugin/loadplugin.hpp"
#include <fstream>
#include <string>
#include "catch2/catch.hpp"
#include "imonitoringplugin/inputfilecontent.hpp"
#include "monitoringplugins/loadplugin/loadpluginprocessors.hpp"

static const std::vector<imonitorplugin::PluginData> sample_data{
    imonitorplugin::PluginData{
        "LoadPlugin", 1558784370, {{"m1", 5}, {"m5", 10}, {"m15", 50}}},
    imonitorplugin::PluginData{
        "LoadPlugin", 1558784375, {{"m1", 5}, {"m5", 10}, {"m15", 50}}},
    imonitorplugin::PluginData{
        "LoadPlugin", 1558784380, {{"m1", 5}, {"m5", 10}, {"m15", 50}}}};

TEST_CASE("LoadPlugin Data acquisition", "[LoadPlugin]") {
  monitoringplugins::loadplugin::LoadPlugin plug{};
  auto data = plug.AcquireData({});

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
  auto processor = monitoringplugins::loadplugin::CreateProcessors().front();
  auto time_series_response = processor->processor()(sample_data);

  std::ifstream ref_stream{std::string{TESTDATA_DIR} +
                           "/loadplugin_time_series_response_ref"};
  std::string ref;
  std::getline(ref_stream, ref);

  REQUIRE(time_series_response == ref);
}
