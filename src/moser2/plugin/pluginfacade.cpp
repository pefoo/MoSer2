#include "pluginfacade.hpp"
#include <memory>
#include "easyloggingpp-9.96.5/src/easylogging++.h"
#include "imonitoringplugin/plugindata.hpp"
#include "utility/datastructure/circularbuffer.hpp"

moser2::plugin::PluginFacade::PluginFacade()
    : buffer_(std::make_unique<utility::datastructure::CircularBuffer<
                 imonitorplugin::PluginData>>(1000)) {}

void moser2::plugin::PluginFacade::Put(const imonitorplugin::PluginData &data) {
  LOG(DEBUG) << data.ToString();
  this->buffer_->Put(data);
}

moser2::plugin::PluginFacade &moser2::plugin::PluginFacade::Instance() {
  static moser2::plugin::PluginFacade instance{};
  return instance;
}

bool moser2::plugin::PluginFacade::GetNext(imonitorplugin::PluginData *data) {
  return this->buffer_->Get(data);
}
