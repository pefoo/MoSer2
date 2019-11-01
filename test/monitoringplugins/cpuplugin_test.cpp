#include "monitoringplugins/cpuplugin/cpuplugin.hpp"
#include <fstream>
#include <string>
#include "catch2/catch.hpp"
#include "imonitoringplugin/inputfilecontent.hpp"
#include "monitoringplugins/cpuplugin/cpupluginprocessors.hpp"

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

static const std::vector<imonitorplugin::PluginData> sample_data{
    imonitorplugin::PluginData{
        "CpuPlugin", 1558784370, {{"core0", 5.0}, {"core1", 10.0}}},
    imonitorplugin::PluginData{
        "CpuPlugin", 1558784375, {{"core0", 5.0}, {"core1", 10.0}}},
    imonitorplugin::PluginData{
        "CpuPlugin", 1558784380, {{"core0", 5.0}, {"core1", 10.0}}}};

TEST_CASE("CpuPlugin Data acquisition", "[CpuPlugin]") {
  monitoringplugins::cpuplugin::CpuPlugin plug{};

  auto data = plug.AcquireData(
      {{"/proc/stat",
        imonitorplugin::InputFileContent{0, snapshot_1, snapshot_2, 1, 1}}})[0];

  REQUIRE(data.plugin_name() == "CpuPlugin");
  REQUIRE(data.data().size() == 4);

  REQUIRE(data.data().at(0).first == "core0");
  REQUIRE(data.data().at(1).first == "core1");
  REQUIRE(data.data().at(2).first == "core2");
  REQUIRE(data.data().at(3).first == "core3");

  using namespace Catch::literals;
  REQUIRE(std::any_cast<float>(data.data().at(0).second) == 6.65393_a);
  REQUIRE(std::any_cast<float>(data.data().at(1).second) == 6.99755_a);
  REQUIRE(std::any_cast<float>(data.data().at(2).second) == 7.48569_a);
  REQUIRE(std::any_cast<float>(data.data().at(3).second) == 7.45299_a);
}

TEST_CASE("CpuPlugin Data processor", "[CpuPlugin]") {
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

  std::ifstream ref_stream{std::string{TESTDATA_DIR} +
                           "/cpuplugin_time_series_response_ref"};
  std::string ref;
  std::getline(ref_stream, ref);

  REQUIRE(time_series_response == ref);
  REQUIRE(avg_response == "7.5");
}
