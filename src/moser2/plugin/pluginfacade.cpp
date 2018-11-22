#include "pluginfacade.hpp"
#include "imonitoringplugin/plugindata.hpp"
#include "utility/datastructure/circularbuffer.hpp"

moser2::plugin::PluginFacade::PluginFacade()
    : buffer(std::make_unique<utility::datastructure::CircularBuffer<
                 imonitorplugin::PluginData>>(1000)) {}

moser2::plugin::PluginFacade &moser2::plugin::PluginFacade::Instance() {
  static moser2::plugin::PluginFacade instance{};
  return instance;
}

bool moser2::plugin::PluginFacade::GetNext(imonitorplugin::PluginData *data) {
  return this->buffer->Get(data);
}
