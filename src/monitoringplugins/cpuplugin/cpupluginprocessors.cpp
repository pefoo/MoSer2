#include "monitoringplugins/cpuplugin/cpupluginprocessors.hpp"
#include <cmath>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "dataprocessorhelper/tablecalchelper.hpp"
#include "monitoringplugins/cpuplugin/constants.hpp"
#include "utility/datastructure/table.hpp"

static constexpr char kTableStart[] = "<table class=default>";
static constexpr char kTableEnd[] = "</table>";
static constexpr char kTableRowStart[] = "<tr class=default>";
static constexpr char kTableRowEnd[] = "</tr>";
static constexpr char kTableHeaderStart[] = "<th class=default>";
static constexpr char kTableHeaderEnd[] = "</th>";
static constexpr char kTableColumnStart[] = "<td class=default>";
static constexpr char kTableColumnEnd[] = "</td>";

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringplugins::cpuplugin::CreateProcessors() {
  return std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>{
      {// The cpu usage chart, base64 encoded
       std::make_shared<monitoringpluginbase::PluginDataProcessor>(
           "%%CPU_TIME_SERIES_DATA%%",
           [](utility::datastructure::Table data) -> std::string {
             if (data.MaxSize() == 0) {
               return "";
             }
             dataprocessorhelper::gnuplot::GnuPlotParameterDict params{};
             params.AddParameter(
                 monitoringplugins::cpuplugin::constants::kGpArgYLabel,
                 "Usage [%]", true);
             params.AddParameter(
                 monitoringplugins::cpuplugin::constants::kGpArgYSeriesCount,
                 std::to_string(data.ColumnCount() - 1), false);

             return dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
                 monitoringplugins::cpuplugin::constants::kGpScriptName, data,
                 params);
           })},
      {// Cpu usage stats per core
       std::make_shared<monitoringpluginbase::PluginDataProcessor>(
           "%%CPU_USAGE_STATS%%",
           [](utility::datastructure::Table data) -> std::string {
             if (data.MaxSize() == 0) {
               return "";
             }
             uint core_count = std::thread::hardware_concurrency();
             if (core_count == 0) {
               return "";
             }

             std::stringstream table{};
             // build the table start with columns: Core, Avg, Min, Max
             table << kTableStart << kTableRowStart << kTableHeaderStart
                   << "Core" << kTableHeaderEnd << kTableHeaderStart << "Avg"
                   << kTableHeaderEnd << kTableHeaderStart << "Min"
                   << kTableHeaderEnd << kTableHeaderStart << "Max"
                   << kTableHeaderEnd << kTableHeaderStart
                   << "&#x3C3;"  // sigma
                   << kTableHeaderEnd << kTableRowEnd;

             for (uint i = 0; i < core_count; ++i) {
               auto filter = dataprocessorhelper::Filter::GetInclusiveFilter(
                   {"core" + std::to_string(i)});
               auto avg =
                   std::round(dataprocessorhelper::Avg<double>(data, filter) *
                              10) /
                   10;
               auto min =
                   std::round(dataprocessorhelper::Min<double>(data, filter) *
                              10) /
                   10;
               auto max =
                   std::round(dataprocessorhelper::Max<double>(data, filter) *
                              10) /
                   10;
               auto std_dev =
                   std::round(
                       dataprocessorhelper::StdDev<double>(data, filter) * 10) /
                   10;

               table << kTableRowStart << kTableColumnStart << i
                     << kTableColumnEnd << kTableColumnStart << avg
                     << kTableColumnEnd << kTableColumnStart << min
                     << kTableColumnEnd << kTableColumnStart << max
                     << kTableColumnEnd << kTableColumnStart << std_dev
                     << kTableColumnEnd << kTableRowEnd;
             }
             table << kTableEnd;
             return table.str();
           })},
      {// The average cpu usage. a single value
       std::make_shared<monitoringpluginbase::PluginDataProcessor>(
           "%%CPU_USAGE_AVERAGE%%",
           [](utility::datastructure::Table data) -> std::string {
             if (data.MaxSize() == 0) {
               return "";
             }
             auto avg =
                 std::round(dataprocessorhelper::Avg<double>(data) * 10) / 10;
             std::stringstream f{};
             f << avg;
             return f.str();
           })}};
}
