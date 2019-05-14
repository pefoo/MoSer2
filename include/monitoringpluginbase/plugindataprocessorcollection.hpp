#ifndef PLUGINDATAPROCESSORCOLLECTION_H
#define PLUGINDATAPROCESSORCOLLECTION_H

#include <memory>
#include "imonitoringplugin/iplugindataprocessorcollection.hpp"

namespace monitoringpluginbase {
///
/// The imonitorplugin::IPluginDataProcessorCollection implementation
///
class PluginDataProcessorCollection
    : public imonitorplugin::IPluginDataProcessorCollection {
 public:
  PluginDataProcessorCollection(
      std::string plugin,
      std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>
      processors = {});

  ///
  /// \copydoc imonitorplugin::IPluginDataProcessorCollection::plugin()
  ///
  std::string plugin() const override;

  ///
  /// \copydoc imonitorplugin::IPluginDataProcessorCollection::processors()
  ///
  std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>
  processors() const override;

 private:
  const std::string plugin_;
  std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>
      processors_;
};

}  // namespace monitoringpluginbase

#endif  // PLUGINDATAPROCESSORCOLLECTION_H
