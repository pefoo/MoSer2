#include "monitoringplugins/cpuplugin/cpupluginprocessors.hpp"
#include <cmath>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "dataprocessorhelper/tablecalchelper.hpp"
#include "monitoringplugins/cpuplugin/constants.hpp"
#include "utility/datastructure/table.hpp"

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
