#include "monitoringplugins/diskplugin/diskplugin.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include "catch2/catch.hpp"
#include "configurationinjector.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "imonitoringplugin/inputfilecontent.hpp"
#include "monitoringplugins/diskplugin/constants.hpp"
#include "monitoringplugins/diskplugin/diskpluginprocessors.hpp"
#include "utility/datastructure/table.hpp"

constexpr char snapshot_1[] =
    "7       0 loop0 50 0 2088 80 0 0 0 0 0 4 28\n"
    "7       1 loop1 140 0 2306 56 0 0 0 0 0 4 16"
    "8       0 sda 136840 26899 7989877 92256 154023 179362 37530369 536928 0 "
    "77032 629032\n"
    "8       1 sda1 89 42 7833 48 1 0 1 0 0 28 48\n"
    "8       2 sda2 40 0 4128 36 0 0 0 0 0 24 36\n"
    "8       3 sda3 64571 25584 3813114 67504 28736 72644 17376566 176632 0 "
    "21888 244084\n"
    "8       4 sda4 72092 1273 4162458 24656 121942 106718 20236360 356352 0 "
    "53836 380908\n"
    "7       8 loop8 33 0 656 4 0 0 0 0 0 0 0\n";

constexpr char snapshot_2[] =
    "7       0 loop0 50 0 2088 80 0 0 0 0 0 4 28\n"
    "7       1 loop1 140 0 2306 56 0 0 0 0 0 4 16\n"
    "8       0 sda 136862 26899 7994517 92268 154484 179623 37640105 537352 0 "
    "77212 629468\n"
    "8       1 sda1 89 42 7833 48 1 0 1 0 0 28 48\n"
    "8       2 sda2 40 0 4128 36 0 0 0 0 0 24 36\n"
    "8       3 sda3 64571 25584 3813115 67504 28850 72672 17376568 176832 0 "
    "22888 244284\n"
    "8       4 sda4 72114 1273 4167098 24668 122285 106951 20263536 356572 0 "
    "53968 381140\n"
    "7       8 loop8 33 0 656 4 0 0 0 0 0 0 0\n";

TEST_CASE("DiskPlugin Data acquisition", "[DiskPlugin]") {
  ConfigurationInjector c{
      monitoringplugins::diskplugin::constants::kPluginName};

  monitoringplugins::diskplugin::DiskPlugin plug{};
  plug.Init();
  auto data = plug.AcquireData(
      {{"/proc/diskstats",
        imonitorplugin::InputFileContent{0, snapshot_1, snapshot_2, 1, 1}}});

  // 2 drives are configured -> 2 data sets acquired
  REQUIRE(data.size() == 2);

  auto sda3_data = *std::find_if(
      data.begin(), data.end(), [](const imonitorplugin::PluginData& data) {
        return std::any_cast<std::string>(data.data().at(3).second) == "sda3";
      });
  auto sda4_data = *std::find_if(
      data.begin(), data.end(), [](const imonitorplugin::PluginData& data) {
        return std::any_cast<std::string>(data.data().at(3).second) == "sda4";
      });

  using namespace Catch::literals;
  REQUIRE(std::any_cast<int64_t>(sda3_data.data().at(0).second) == 512);
  REQUIRE(std::any_cast<int64_t>(sda3_data.data().at(1).second) == 1024);
  REQUIRE(std::any_cast<double>(sda3_data.data().at(2).second) == 100.0_a);

  REQUIRE(std::any_cast<int64_t>(sda4_data.data().at(0).second) == 2375680);
  REQUIRE(std::any_cast<int64_t>(sda4_data.data().at(1).second) == 13914112);
  REQUIRE(std::any_cast<double>(sda4_data.data().at(2).second) == 13.2_a);
}

TEST_CASE("DiskPlugin Data processor", "[DiskPlugin]") {
  ConfigurationInjector c{
      monitoringplugins::diskplugin::constants::kPluginName};
  utility::datastructure::Table sample_data{"DiskPlugin"};
  sample_data.AddColumn(
      utility::datastructure::DataColumn<int>{"bytes_read", {5, 5, 5}});
  sample_data.AddColumn(
      utility::datastructure::DataColumn<int>{"bytes_written", {10, 10, 10}});
  sample_data.AddColumn(
      utility::datastructure::DataColumn<int>{"utilization", {50, 50, 50}});
  sample_data.AddColumn(utility::datastructure::DataColumn<int>{
      "timestamp", {1558784370, 1558784375, 1558784380}});
  sample_data.AddColumn(utility::datastructure::DataColumn<std::string>{
      "label", {"sda3", "sda3", "sda3"}});
  dataprocessorhelper::gnuplot::GnuPlotBackend::instance().set_mock_call(true);
  auto processors = monitoringplugins::diskplugin::CreateProcessors();
  auto time_series_processor = processors.front();

  auto time_series_response = time_series_processor->processor()(sample_data);

  REQUIRE(time_series_response ==
          "ImRpc2tfY2hhcnRzLmdwIgoidGl0bGU9J3NkYTMnIgo=");
}
