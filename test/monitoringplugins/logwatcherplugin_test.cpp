#include "monitoringplugins/logwatcherplugin/logwatcherplugin.hpp"
#include <catch2/catch.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include "configurationinjector.hpp"
#include "monitoringplugins/logwatcherplugin/constants.hpp"
#include "monitoringplugins/logwatcherplugin/logwatcherpluginprocessors.hpp"

static constexpr char kFileName[] = "unit_test_log_file.log";
static const std::vector<imonitorplugin::PluginData> sample_data{
    imonitorplugin::PluginData{
        monitoringplugins::logwatcherplugin::constants::kPluginName,
        1558784370,
        {{monitoringplugins::logwatcherplugin::constants::kDbFieldEntry,
          std::string{"Some error"}},
         {monitoringplugins::logwatcherplugin::constants::kDbFieldTags,
          std::string{"errors"}}}},
    imonitorplugin::PluginData{
        monitoringplugins::logwatcherplugin::constants::kPluginName,
        1558784375,
        {{monitoringplugins::logwatcherplugin::constants::kDbFieldEntry,
          std::string{"No error"}},
         {monitoringplugins::logwatcherplugin::constants::kDbFieldTags,
          std::string{"another_tag"}}}},
    imonitorplugin::PluginData{
        monitoringplugins::logwatcherplugin::constants::kPluginName,
        1558784380,
        {{monitoringplugins::logwatcherplugin::constants::kDbFieldEntry,
          std::string{"Error and another tag"}},
         {monitoringplugins::logwatcherplugin::constants::kDbFieldTags,
          std::string{"errors,another_tag"}}}}};

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

TEST_CASE("LogWatcherPlugin data processor", "[LogWatcherPlugin]") {
  auto processors = monitoringplugins::logwatcherplugin::CreateProcessors();

  // One tag is configred -> one processor provided
  REQUIRE(processors.size() == 1);
  auto processor = processors.at(0);
  REQUIRE(processor->key() == "%%LOG_TABLE_errors%%");
  auto result = processor->processor()(sample_data);
  REQUIRE(result.find("Some error") != std::string::npos);
  REQUIRE(result.find("Error and another tag") != std::string::npos);
  REQUIRE(result.find("No error") == std::string::npos);
}
