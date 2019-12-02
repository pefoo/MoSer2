#include "monitoringplugins/networkplugin/networkpluginprocessors.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "monitoringplugins/networkplugin/constants.hpp"
#include "utility/datastructure/table.hpp"

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringplugins::networkplugin::CreateProcessors() {
  return std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>{
      {// The cpu usage chart, base64 encoded
       std::make_shared<monitoringpluginbase::PluginDataProcessor>(
           "%%NETWORK_TIME_SERIES_DATA%%",
           [](utility::datastructure::Table data) -> std::string {
             if (data.MaxSize() == 0) return "";
             dataprocessorhelper::gnuplot::GnuPlotParameterDict params{};
             return dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
                 monitoringplugins::networkplugin::constants::
                     kPluginGpScirptName,
                 data, params);
           })}};
}
