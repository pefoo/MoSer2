#include "monitoringplugins/memoryplugin/memorypluginprocessors.hpp"
#include <memory>
#include <string>
#include <vector>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "monitoringplugins/memoryplugin/constants.hpp"
#include "utility/datastructure/table.hpp"

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringplugins::memoryplugin::CreateProcessors() {
  return std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>{
      {std::make_shared<monitoringpluginbase::PluginDataProcessor>(
          "%%MEMORY_TIME_SERIES_DATA%%",
          [](utility::datastructure::Table data) -> std::string {
            if (data.MaxSize() == 0) return "";
            dataprocessorhelper::gnuplot::GnuPlotParameterDict params{};
            return dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
                monitoringplugins::memoryplugin::constants::kGpScriptName, data,
                params);
          })}};
}
