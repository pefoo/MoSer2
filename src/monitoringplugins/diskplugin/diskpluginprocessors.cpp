#include "monitoringplugins/diskplugin/diskpluginprocessors.hpp"
#include <memory>
#include <string>
#include <vector>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "monitoringplugins/diskplugin/constants.hpp"

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringplugins::diskplugin::CreateProcessors() {
  return std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>{
      // TODO find a way to match token name and drive data
      {std::make_shared<monitoringpluginbase::PluginDataProcessor>(
          "%%DISK_TIME_SERIES_DATA%%",
          [](std::vector<imonitorplugin::PluginData> records) -> std::string {
            if (records.size() == 0) {
              return "";
            }
            auto record_filter = [](const std::string& key) {
              return key.find("sda4_") == 0;
            };
            dataprocessorhelper::gnuplot::GnuPlotParameterDict params{};

            return dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
                monitoringplugins::diskplugin::constants::kGpScriptName,
                records,
                monitoringplugins::diskplugin::constants::kGpArgFileName,
                params, ";", record_filter);
          })}};
}
