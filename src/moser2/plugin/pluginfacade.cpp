#include "pluginfacade.hpp"
#include <memory>
#include "easyloggingpp-9.96.5/easylogging++.h"
#include "imonitoringplugin/plugindata.hpp"
#include "utility/datastructure/circularbuffer.hpp"

static constexpr int kBufferSize = 1000;
static constexpr int kBufferWarnSize = 950;

moser2::plugin::PluginFacade::PluginFacade()
    : buffer_(std::make_unique<utility::datastructure::CircularBuffer<
                  imonitorplugin::PluginData>>(kBufferSize)) {}

void moser2::plugin::PluginFacade::Put(const imonitorplugin::PluginData &data) {
  LOG(TRACE) << data.ToString();
  this->buffer_->Put(data);
  if (this->buffer_->is_full()) {
    LOG(ERROR) << "The plugin data buffer is full!";
  } else if (this->buffer_->size() >= kBufferWarnSize) {
    LOG(WARNING) << "The plugin data buffer is almost full ["
                 << this->buffer_->size() << "/" << kBufferSize << "]";
  }
}

moser2::plugin::PluginFacade &moser2::plugin::PluginFacade::Instance() {
  static moser2::plugin::PluginFacade instance{};
  return instance;
}

bool moser2::plugin::PluginFacade::GetNext(imonitorplugin::PluginData *data) {
  return this->buffer_->Get(data);
}
