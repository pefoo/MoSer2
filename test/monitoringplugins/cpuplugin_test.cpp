#include "monitoringplugins/cpuplugin/cpuplugin.hpp"
#include <fstream>
#include <string>
#include <thread>
#include "catch2/catch.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "imonitoringplugin/inputfilecontent.hpp"
#include "monitoringplugins/cpuplugin/cpupluginprocessors.hpp"
#include "utility/datastructure/table.hpp"

constexpr char snapshot_1[] =
    "cpu  118170 409 39814 1626469 1218 0 3923 0 0 0\n"
    "cpu0 30052 68 10520 406308 128 0 843 0 0 0\n"
    "cpu1 29380 78 9844 406093 256 0 1399 0 0 0\n"
    "cpu2 29421 199 9719 406715 675 0 804 0 0 0\n"
    "cpu3 29316 62 9730 407352 158 0 876 0 0 0\n"
    "intr 3368371 9 0 0 0 0 0 0 0 1 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
    "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
    "0 0 0 0 0 0 0 0\n"
    "ctxt 17605524\n"
    "btime 1559899952\n"
    "processes 9306\n"
    "procs_running 1\n"
    "procs_blocked 0\n"
    "softirq 2881558 677550 677135 851 329044 120560 0 3649 576054 0 496715\n";

constexpr char snapshot_2[] =
    "cpu  120913 409 40712 1675080 1230 0 4014 0 0 0\n"
    "cpu0 30681 68 10745 418527 129 0 859 0 0 0\n"
    "cpu1 30051 78 10037 418254 257 0 1449 0 0 0\n"
    "cpu2 30119 199 9980 418839 683 0 818 0 0 0\n"
    "cpu3 30061 62 9949 419459 159 0 886 0 0 0\n"
    "intr 3433490 9 0 0 0 0 0 0 0 1 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
    "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 "
    "0 0 0 0 0 0 0 0\n"
    "ctxt 18020130\n"
    "btime 1559899952\n"
    "processes 9520\n"
    "procs_running 1\n"
    "procs_blocked 0\n"
    "softirq 2939767 689520 694940 876 329936 121162 0 4056 590473 0 508804";

TEST_CASE("CpuPlugin Data acquisition", "[CpuPlugin]") {
  monitoringplugins::cpuplugin::CpuPlugin plug{};
  plug.Init();

  auto data = plug.AcquireData(
      {{"/proc/stat",
        imonitorplugin::InputFileContent{0, snapshot_1, snapshot_2, 1, 1}}})[0];

  REQUIRE(data.plugin_name() == "CpuPlugin");
  auto core_count = std::thread::hardware_concurrency();
  if (core_count > 4) {
    FAIL("This stupid test should not run on machines with more than 4 cores.");
  }
  REQUIRE(data.data().size() == core_count);

  using namespace Catch::literals;
  constexpr double results[] = {6.6463, 6.98991, 7.42465, 7.44534};
  for (uint i = 0; i < core_count; ++i) {
    REQUIRE(data.data().at(i).first == "core" + std::to_string(i));
    REQUIRE(std::any_cast<float>(data.data().at(i).second) ==
            Approx(results[i]));
  }
}

TEST_CASE("CpuPlugin Data processor", "[CpuPlugin]") {
  utility::datastructure::Table sample_data{"CpuPlugion"};
  sample_data.AddColumn(
      utility::datastructure::DataColumn<double>{"core0", {5.0, 5.0, 5.0}});
  sample_data.AddColumn(
      utility::datastructure::DataColumn<double>{"core1", {10.0, 10.0, 10.0}});
  sample_data.AddColumn(utility::datastructure::DataColumn<int>{
      "timestamp", {1558784370, 1558784375, 1558784380}});
  dataprocessorhelper::gnuplot::GnuPlotBackend::instance().set_mock_call(true);
  auto processors = monitoringplugins::cpuplugin::CreateProcessors();
  auto time_series = *std::find_if(
      processors.begin(), processors.end(),
      [](std::shared_ptr<imonitorplugin::IPluginDataProcessor> p) -> bool {
        return p->key() == "%%CPU_TIME_SERIES_DATA%%";
      });
  auto avg = *std::find_if(
      processors.begin(), processors.end(),
      [](std::shared_ptr<imonitorplugin::IPluginDataProcessor> p) -> bool {
        return p->key() == "%%CPU_USAGE_AVERAGE%%";
      });

  auto time_series_response = time_series->processor()(sample_data);
  auto avg_response = avg->processor()(sample_data);

  REQUIRE(time_series_response ==
          "ImNwdV9jaGFydHMuZ3AiCiJ5X2xhYmVsPSdVc2FnZSBbJV0nO3lfc2VyaWVzX2NvdW50"
          "PTIiCg==");
  REQUIRE(avg_response == "7.5");
}
