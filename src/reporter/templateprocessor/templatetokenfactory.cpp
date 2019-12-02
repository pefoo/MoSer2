#include "reporter/templateprocessor/templatetokenfactory.hpp"
#include <memory>
#include <utility>
#include <vector>
#include "utility/datastructure/table.hpp"

reporter::templateprocessor::TemplateTokenFactory::TemplateTokenFactory(
    std::shared_ptr<persistenceservice::IDataAdapter> data_adapter)
    : data_adapter_(std::move(data_adapter)) {}

std::vector<reporter::templateprocessor::TemplateToken>
reporter::templateprocessor::TemplateTokenFactory::BuildTokens(
    const imonitorplugin::IPluginDataProcessorCollection* const
        processor_collection,
    int64_t min_age) {
  data_[processor_collection->plugin()] =
      this->data_adapter_->LoadTable(processor_collection->plugin(), min_age);
  std::vector<reporter::templateprocessor::TemplateToken> tokens;

  for (const auto& processor : processor_collection->processors()) {
    tokens.emplace_back(
        processor->key(),
        [processor, &data = this->data_[processor_collection->plugin()]]() {
          return processor->processor()(data);
        });
  }

  return tokens;
}
