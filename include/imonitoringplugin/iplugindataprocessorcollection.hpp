#ifndef IPLUGINDATAPROCESSORCOLLECTION_H
#define IPLUGINDATAPROCESSORCOLLECTION_H

#include <imonitoringplugin/iplugindataprocessor.hpp>
#include <memory>
#include <string>
#include <vector>

namespace imonitorplugin {

///
/// \brief A collection of plugin data processors
/// \details Load the implementation of this class from a monitoring plugin
/// library to get the defined processors.
///
class IPluginDataProcessorCollection {
 public:
  ///
  /// \brief ~IPluginDataProcessorCollection
  ///
  virtual ~IPluginDataProcessorCollection() {}

  ///
  /// \brief Get the name of the plugin this collection belongs to
  /// \return The plugin name
  ///
  virtual std::string plugin() const = 0;

  ///
  /// \brief Get the defined processors.
  /// \details Note that this list might be empty, if the plugin does not define
  /// a processor \return A vector with pointers to the processors
  ///
  virtual std::vector<std::shared_ptr<IPluginDataProcessor>> processors()
      const = 0;
};

///
/// \brief A typedef for the plugin data processor factory function
/// (constructor)
/// \note This type is required when loading the processor
/// collection as a plugin
///
typedef imonitorplugin::IPluginDataProcessorCollection* create_processors_t();

///
/// \brief A typedef for the plugin data processor factory function (destructor)
/// \note This type is required when loading the processor collection as a
/// plugin
///
typedef void destroy_processors_t(
    imonitorplugin::IPluginDataProcessorCollection*);
}  // namespace imonitorplugin

#endif  // IPLUGINDATAPROCESSORCOLLECTION_H
