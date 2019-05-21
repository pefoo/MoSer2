#include "monitoringplugins/cpuplugin/cpupluginprocessors.hpp"
#include <memory>
#include <string>
#include <vector>

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringplugins::cpuplugin::CreateProcessors() {
  // TODO Remove this dummy
  return std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>{
      {std::make_shared<monitoringpluginbase::PluginDataProcessor>(
          "%%foobar%%",
          [](std::vector<imonitorplugin::PluginData> records) -> std::string {
            return std::to_string(records.size());
          })}};
}
