#ifndef PLUGINDATAPROCESSORCOLLECTION_H
#define PLUGINDATAPROCESSORCOLLECTION_H

#include <memory>
#include "imonitoringplugin/iplugindataprocessorcollection.hpp"
#include "imonitoringplugin/constants.hpp"

namespace monitoringpluginbase {
///*
// * Macros to define the default function factories.
// * Pass the plugin type as argument.
// * Note that these macros may not be placed inside a class!
// */
//#define CREATE_PROCESSORS(plugin_name, processors) \
//  extern "C" ::imonitorplugin::IPluginDataProcessorCollection* create_processors() { return new PluginDataProcessorCollection{plugin_name, processors} }

#define DESYTROY_PROCESSORS() \
  extern "C" void DATA_PROCESSOR_DESTRUCTOR(imonitorplugin::IPluginDataProcessorCollection* p) { delete p; }

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
