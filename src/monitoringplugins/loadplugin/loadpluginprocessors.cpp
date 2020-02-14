#include "monitoringplugins/loadplugin/loadpluginprocessors.hpp"
#include <memory>
#include <string>
#include <vector>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "utility/datastructure/table.hpp"

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringplugins::loadplugin::CreateProcessors() {
  return std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>{
      {std::make_shared<monitoringpluginbase::PluginDataProcessor>(
          kTokenLoadTimeSeriesData,
          [](utility::datastructure::Table data) -> std::string {
            if (data.MaxSize() == 0) {
              return "";
            }
            dataprocessorhelper::gnuplot::GnuPlotParameterDict params{};
            return dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
                "load_charts.gp", data, params);
          })}};
}
