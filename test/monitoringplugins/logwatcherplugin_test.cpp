#include "monitoringplugins/logwatcherplugin/logwatcherplugin.hpp"
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include "configurationinjector.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "monitoringplugins/logwatcherplugin/constants.hpp"
#include "monitoringplugins/logwatcherplugin/logwatcherpluginprocessors.hpp"
#include "utility/datastructure/table.hpp"

static constexpr char kFileName[] = "unit_test_log_file.log";

void PrepareLogFile() {
  std::stringstream t{};
  t << LIBRARY_DIR << "/" << kFileName;
  std::ofstream s{t.str(), std::ios_base::trunc};
}

void PopulateLogFile() {
  std::stringstream s{};
  s << TESTDATA_DIR << "/" << kFileName;
  std::stringstream t{};
  t << LIBRARY_DIR << "/" << kFileName;
  std::filesystem::copy_file(s.str(), t.str(),
                             std::filesystem::copy_options::overwrite_existing);
}

TEST_CASE("LogWatcherPlugin data acquisition", "[LogWatcherPlugin]") {
  PrepareLogFile();
  ConfigurationInjector c{
      monitoringplugins::logwatcherplugin::constants::kPluginName};
  monitoringplugins::logwatcherplugin::LogWatcherPlugin plug{};
  plug.Init();
  auto messages = plug.DoSanityCheck();
  REQUIRE(messages.size() == 0);

  // Initial run sets file positions
  auto data = plug.AcquireData({});
  REQUIRE(data.size() == 0);
  PopulateLogFile();

  data = plug.AcquireData({});
  // 5 Lines in total. 5 are selected by the "all" tag. Error tag selects 2
  REQUIRE(data.size() == 7);
  auto error1 = std::any_cast<std::string>(data[5].data()[0].second);
  auto error2 = std::any_cast<std::string>(data[6].data()[0].second);
  REQUIRE(error1 == "Error bar");
  REQUIRE(error2 == "bar Error foo");
}
using namespace monitoringplugins::logwatcherplugin::constants;

TEST_CASE("LogWatcherPlugin data processor", "[LogWatcherPlugin]") {
  ConfigurationInjector c{
      monitoringplugins::logwatcherplugin::constants::kPluginName};
  utility::datastructure::Table sample_data{"LogWatcherPlugin"};
  sample_data.AddColumn(utility::datastructure::DataColumn<std::string>{
      kDbFieldEntry, {"Some error", "No error", "Error and another tag"}});
  sample_data.AddColumn(utility::datastructure::DataColumn<std::string>{
      kDbFieldTags, {"errors", "another_tag", "errors,another_tag"}});
  sample_data.AddColumn(utility::datastructure::DataColumn<int>{
      "timestamp", {1558784370, 1558784375, 1558784380}});

  auto processors = monitoringplugins::logwatcherplugin::CreateProcessors();

  // One tag is configred -> two processors provided
  REQUIRE(processors.size() == 2);

  SECTION("Log table processor") {
    auto processor = processors.at(0);
    REQUIRE(processor->key() == "%%LOG_TABLE_errors%%");
    auto result = processor->processor()(sample_data);
    REQUIRE(result.find("Some error") != std::string::npos);
    REQUIRE(result.find("Error and another tag") != std::string::npos);
    REQUIRE(result.find("No error") == std::string::npos);
  }

  SECTION("Log chart processor") {
    dataprocessorhelper::gnuplot::GnuPlotBackend::instance().set_mock_call(
        true);
    auto processor = processors.at(1);
    REQUIRE(processor->key() == "%%LOG_CHART_errors%%");
    auto result = processor->processor()(sample_data);
    REQUIRE(result ==
            "ImxvZ3dhdGNoZXJfY2hhcnRzLmdwIgoieV9sYWJlbD0nSW52YWxpZCBzc2ggdXNlci"
            "BuYW1lcyciCg==");
  }
}
