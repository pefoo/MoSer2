#include "monitoringpluginbase/plugindataprocessor.hpp"
#include <string>
#include <utility>

monitoringpluginbase::PluginDataProcessor::PluginDataProcessor(
    std::string token,
    imonitorplugin::IPluginDataProcessor::DataProcessor processor)
    : token_(std::move(token)), processor_(std::move(processor)) {}

imonitorplugin::IPluginDataProcessor::DataProcessor
monitoringpluginbase::PluginDataProcessor::processor() const {
  return this->processor_;
}

std::string monitoringpluginbase::PluginDataProcessor::key() const {
  return this->token_;
}
