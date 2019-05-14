#include "monitoringpluginbase/plugindataprocessorcollection.hpp"
#include <utility>

monitoringpluginbase::PluginDataProcessorCollection::
    PluginDataProcessorCollection(
        std::string plugin,
        std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor> >
            processors)
    : plugin_(std::move(plugin)), processors_(processors) {}

std::string monitoringpluginbase::PluginDataProcessorCollection::plugin()
    const {
  return this->plugin_;
}

std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor> >
monitoringpluginbase::PluginDataProcessorCollection::processors() const {
  return this->processors_;
}
