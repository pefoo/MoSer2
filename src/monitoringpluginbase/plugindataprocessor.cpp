#include "monitoringpluginbase/plugindataprocessor.hpp"

monitoringpluginbase::PluginDataProcessor::PluginDataProcessor(
    std::string token,
    imonitorplugin::IPluginDataProcessor::DataProcessor processor)
    : token_(token), processor_(processor) {}

imonitorplugin::IPluginDataProcessor::DataProcessor
monitoringpluginbase::PluginDataProcessor::processor() const {
  return this->processor_;
}

std::string monitoringpluginbase::PluginDataProcessor::key() const {
  return this->token_;
}
