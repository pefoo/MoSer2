#ifndef PLUGINDATAPROCESSORCOLLECTION_H
#define PLUGINDATAPROCESSORCOLLECTION_H

#include <memory>
#include "imonitoringplugin/constants.hpp"
#include "imonitoringplugin/iplugindataprocessorcollection.hpp"

namespace monitoringpluginbase {

///
/// \brief A macro to create the factory function for the processor collection
/// \details This function is used to load the processors as a plugin.
/// \note To actually expose any processor, use this macro and pass your
/// processors. Do not place this macro inside a class or function.
/// \param plugin_name The name of the plugin, this collection
/// belongs to
/// \param processors A vector with processors (see constructor of
/// monitoringpluginbase::PluginDataProcessorCollection
///
#define CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(plugin_name, processors)  \
  extern "C" ::imonitorplugin::IPluginDataProcessorCollection*         \
  DATA_PROCESSOR_CONSTRUCTOR() {                                       \
    return new ::monitoringpluginbase::PluginDataProcessorCollection { \
      plugin_name, processors                                          \
    }                                                                  \
  }

///
/// \brief A macro to create the factory function for the processor collection
/// destructor
/// \details This function is used to destroy the processor
/// collection, when loaded as a plugin
/// \note Required, when processors are exposed. Do not place this macro inside
/// a class or function.
///
#define CREATE_PROCESSOR_DESTRUCTOR_FACTORY()                \
  extern "C" void DATA_PROCESSOR_DESTRUCTOR(                 \
      ::imonitorplugin::IPluginDataProcessorCollection* p) { \
    delete p;                                                \
  }

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
