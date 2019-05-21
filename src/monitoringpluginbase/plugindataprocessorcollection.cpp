#include "monitoringpluginbase/plugindataprocessorcollection.hpp"
#include <string>
#include <utility>

monitoringpluginbase::PluginDataProcessorCollection::
    PluginDataProcessorCollection(std::string plugin,
                                  ProcessorVector processors)
    : plugin_(std::move(plugin)), processors_(processors) {}

std::string monitoringpluginbase::PluginDataProcessorCollection::plugin()
    const {
  return this->plugin_;
}

monitoringpluginbase::PluginDataProcessorCollection::ProcessorVector
monitoringpluginbase::PluginDataProcessorCollection::processors() const {
  return this->processors_;
}
