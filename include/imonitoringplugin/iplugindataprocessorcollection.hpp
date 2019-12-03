#ifndef IPLUGINDATAPROCESSORCOLLECTION_H
#define IPLUGINDATAPROCESSORCOLLECTION_H
// LCOV_EXCL_START

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
  virtual ~IPluginDataProcessorCollection() = default;

  ///
  /// \brief Get the name of the plugin this collection belongs to
  /// \return The plugin name
  ///
  [[nodiscard]] virtual std::string plugin() const = 0;

  ///
  /// \brief Get the defined processors.
  /// \details Note that this list might be empty, if the plugin does not define
  /// a processor \return A vector with pointers to the processors
  ///
  [[nodiscard]] virtual std::vector<std::shared_ptr<IPluginDataProcessor>>
  processors() const = 0;
};

}  // namespace imonitorplugin
// LCOV_EXCL_STOP
#endif  // IPLUGINDATAPROCESSORCOLLECTION_H
