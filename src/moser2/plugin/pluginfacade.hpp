#ifndef PLUGINFACADE_H
#define PLUGINFACADE_H

#include <memory>
#include "imonitoringplugin/plugindata.hpp"
#include "utility/datastructure/circularbuffer.hpp"

namespace moser2 {
namespace plugin {
///
/// \brief TODO implement the static facade using a circular buffer
///
class PluginFacade {
 public:
  static PluginFacade& Instance();

  bool GetNext(imonitorplugin::PluginData* data);

 private:
  PluginFacade();
  friend class PluginController;
  void Put(const imonitorplugin::PluginData& data);
  std::unique_ptr<
      utility::datastructure::CircularBuffer<imonitorplugin::PluginData>>
      buffer_;
};
}  // namespace plugin
}  // namespace moser2
#endif  // PLUGINFACADE_H
