#include "monitoringplugins/cpuplugin/cpupluginprocessors.hpp"
#include <memory>
#include <string>
#include <vector>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "monitoringplugins/cpuplugin/constants.hpp"

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringplugins::cpuplugin::CreateProcessors() {
  return std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>{
      {// The cpu usage chart, base64 encoded
       std::make_shared<monitoringpluginbase::PluginDataProcessor>(
           "%%CPU_TIME_SERIES_DATA%%",
           [](std::vector<imonitorplugin::PluginData> records) -> std::string {
             if (records.size() == 0) {
               return "";
             }
             dataprocessorhelper::gnuplot::GnuPlotParameterDict params{};
             params.AddParameter(
                 monitoringplugins::cpuplugin::constants::kGpArgYLabel,
                 "Usage [%]", true);
             params.AddParameter(
                 monitoringplugins::cpuplugin::constants::kGpArgYSeriesCount,
                 std::to_string(records.front().data().size()), false);

             return dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
                 monitoringplugins::cpuplugin::constants::kGpScriptName,
                 records,
                 monitoringplugins::cpuplugin::constants::kGpArgFileName,
                 params);
           })},
      {// The average cpu usage. a single value
       std::make_shared<monitoringpluginbase::PluginDataProcessor>(
           "%%CPU_USAGE_AVERAGE%%",
           [](std::vector<imonitorplugin::PluginData> records) -> std::string {
             if (records.size() == 0) {
               return "";
             }
             double core_count = records.front().data().size();
             double total = 0.0;
             for (const auto& record : records) {
               for (const auto& v : record.data()) {
                 // We know its double right now. See sqlite3 data types.
                 total += std::any_cast<double>(v.second);
               }
             }
             return std::to_string(total / core_count);
           })}};
}
