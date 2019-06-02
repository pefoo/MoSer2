#include "reporter/templateprocessor/templatetokenfactory.hpp"
#include <memory>
#include <utility>
#include <vector>

reporter::templateprocessor::TemplateTokenFactory::TemplateTokenFactory(
    std::shared_ptr<persistenceservice::IDataAdapter> data_adapter)
    : data_adapter_(std::move(data_adapter)) {}

std::vector<reporter::templateprocessor::TemplateToken>
reporter::templateprocessor::TemplateTokenFactory::BuildTokens(
    const imonitorplugin::IPluginDataProcessorCollection* const
        processor_collection,
    int64_t min_age) {
  auto data =
      this->data_adapter_->Load(processor_collection->plugin(), min_age);
  std::vector<reporter::templateprocessor::TemplateToken> tokens;

  for (const auto& processor : processor_collection->processors()) {
    tokens.emplace_back(
        processor->key(),
        [processor, data]() { return processor->processor()(data); });
  }

  return tokens;
}
