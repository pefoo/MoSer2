#include "monitoringplugins/loadplugin/loadpluginprocessors.hpp"
#include <memory>
#include <string>
#include <vector>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringplugins::loadplugin::CreateProcessors() {
  return std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>{
      {std::make_shared<monitoringpluginbase::PluginDataProcessor>(
          "%%LOAD_TIME_SERIES_DATA%%",
          [](std::vector<imonitorplugin::PluginData> records) -> std::string {
            if (records.size() == 0) {
              return "";
            }
            dataprocessorhelper::gnuplot::GnuPlotParameterDict params{};
            return dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
                "load_charts.gp", records, "file_name", params);
          })}};
};
