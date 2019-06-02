#include "monitoringplugins/diskplugin/diskpluginprocessors.hpp"
#include <memory>
#include <string>
#include <vector>
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include "dataprocessorhelper/settingshelper.hpp"
#include "monitoringplugins/diskplugin/constants.hpp"
#include "settingsprovider/isettingsprovider.hpp"

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringplugins::diskplugin::CreateProcessors() {
  auto settings = dataprocessorhelper::GetPluginSettings("DiskPlugin");
  std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>
      processors{};

  if (settings) {
    auto devices = std::stringstream{settings->GetValue("Devices", "")};
    std::string device;
    std::vector<std::string> device_list;
    while (std::getline(devices, device, ';')) {
      processors.push_back(
          std::make_shared<monitoringpluginbase::PluginDataProcessor>(
              "%%DISK_" + device + "_TIME_SERIES_DATA%%",
              [device](std::vector<imonitorplugin::PluginData> records)
                  -> std::string {
                if (records.empty()) {
                  return "";
                }
                auto record_filter = [device](const std::string& key) {
                  return key.find(device) == 0;
                };
                dataprocessorhelper::gnuplot::GnuPlotParameterDict params{};

                return dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
                    monitoringplugins::diskplugin::constants::kGpScriptName,
                    records,
                    monitoringplugins::diskplugin::constants::kGpArgFileName,
                    params, ";", record_filter);
              }));
    }
  }

  return processors;
}
