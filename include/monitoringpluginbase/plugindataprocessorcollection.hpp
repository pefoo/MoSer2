#ifndef PLUGINDATAPROCESSORCOLLECTION_H
#define PLUGINDATAPROCESSORCOLLECTION_H
// LCOV_EXCL_START
#include <memory>
#include <string>
#include <vector>
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
#ifndef LINKED_PLUGIN
#define CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(plugin_name, processors) \
  extern "C" ::imonitorplugin::IPluginDataProcessorCollection*        \
  DATA_PROCESSOR_CONSTRUCTOR() {                                      \
    return new ::monitoringpluginbase::PluginDataProcessorCollection{ \
        plugin_name, processors};                                     \
  }
#else
#define CREATE_PROCESSOR_CONSTRUCTOR_FACTORY(plugin_name, processors)
#endif
///
/// \brief A macro to create the factory function for the processor collection
/// destructor
/// \details This function is used to destroy the processor
/// collection, when loaded as a plugin
/// \note Required, when processors are exposed. Do not place this macro inside
/// a class or function.
///
#ifndef LINKED_PLUGIN
#define CREATE_PROCESSOR_DESTRUCTOR_FACTORY()                \
  extern "C" void DATA_PROCESSOR_DESTRUCTOR(                 \
      ::imonitorplugin::IPluginDataProcessorCollection* p) { \
    delete p;                                                \
  }
#else
#define CREATE_PROCESSOR_DESTRUCTOR_FACTORY()
#endif
///
/// The imonitorplugin::IPluginDataProcessorCollection implementation
///
class PluginDataProcessorCollection
    : public imonitorplugin::IPluginDataProcessorCollection {
 public:
  using ProcessorVector =
      std::vector<std::shared_ptr<imonitorplugin::IPluginDataProcessor>>;

  explicit PluginDataProcessorCollection(std::string plugin,
                                         ProcessorVector processors = {});

  ///
  /// \copydoc imonitorplugin::IPluginDataProcessorCollection::plugin()
  ///
  [[nodiscard]] std::string plugin() const override;

  ///
  /// \copydoc imonitorplugin::IPluginDataProcessorCollection::processors()
  ///
  [[nodiscard]] ProcessorVector processors() const override;

 private:
  const std::string plugin_;
  ProcessorVector processors_;
};
// LCOV_EXCL_STOP
}  // namespace monitoringpluginbase

#endif  // PLUGINDATAPROCESSORCOLLECTION_H
