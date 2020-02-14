#include "monitoringplugins/networkplugin/networkpluginprocessors.hpp"
#include <cmath>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "dataprocessorhelper/tablecalchelper.hpp"
#include "monitoringplugins/networkplugin/constants.hpp"
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
monitoringplugins::networkplugin::CreateProcessors() {
  return std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>{
      {// The network usage chart, base64 encoded
       std::make_shared<monitoringpluginbase::PluginDataProcessor>(
           "%%NETWORK_TIME_SERIES_DATA%%",
           [](utility::datastructure::Table data) -> std::string {
             if (data.MaxSize() == 0) return "";
             dataprocessorhelper::gnuplot::GnuPlotParameterDict params{};
             return dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
                 monitoringplugins::networkplugin::constants::
                     kPluginGpScirptName,
                 data, params);
           })},
      {// Network usage statistics
       std::make_shared<monitoringpluginbase::PluginDataProcessor>(
           "%%NETWORK_USAGE_STATS%%",
           [](utility::datastructure::Table data) -> std::string {
             if (data.MaxSize() == 0) {
               return "";
             }

             std::stringstream table{};
             // build the table start with columns: Core, Avg, Min, Max
             table << kTableStart << kTableRowStart << kTableHeaderStart
                   << "Type" << kTableHeaderEnd << kTableHeaderStart
                   << "Avg [kB]" << kTableHeaderEnd << kTableHeaderStart
                   << "Min [kB]" << kTableHeaderEnd << kTableHeaderStart
                   << "Max [kB]" << kTableHeaderEnd << kTableHeaderStart
                   << "&#x3C3; [kB]"  // sigma
                   << kTableHeaderEnd << kTableRowEnd;

             for (auto type : {"rx", "tx"}) {
               auto filter =
                   dataprocessorhelper::Filter::GetInclusiveFilter({type});
               // nice try clang format...
               // clang-format off
               // Calculate and convert the stats to kibibyte
               auto avg = std::round(
                     dataprocessorhelper::Avg<int>(data, filter) * 10 * 0.000976562) / 10;
               auto min = std::round(
                     dataprocessorhelper::Min<int>(data, filter) * 10 * 0.000976562) / 10;
               auto max = std::round(
                     dataprocessorhelper::Max<int>(data, filter) * 10 * 0.000976562) / 10;
               auto std_dev = std::round(
                     dataprocessorhelper::StdDev<int>(data, filter) * 10 * 0.000976562) / 10;
               // clang-format on

               table << kTableRowStart << kTableColumnStart << type
                     << kTableColumnEnd << kTableColumnStart << avg
                     << kTableColumnEnd << kTableColumnStart << min
                     << kTableColumnEnd << kTableColumnStart << max
                     << kTableColumnEnd << kTableColumnStart << std_dev
                     << kTableColumnEnd << kTableRowEnd;
             }
             table << kTableEnd;
             return table.str();
           })},
  };
}
