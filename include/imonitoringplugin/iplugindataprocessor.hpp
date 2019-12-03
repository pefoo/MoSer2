#ifndef IPLUGINDATAPROCESSOR_H
#define IPLUGINDATAPROCESSOR_H
// LCOV_EXCL_START

#include <functional>
#include <string>
#include <vector>
#include "imonitoringplugin/plugindata.hpp"
#include "utility/datastructure/table.hpp"

namespace imonitorplugin {

///
/// \brief The PluginDataProcessor interface
/// \details Holds a processor, that is able to convert a vector of plugin data
/// objects into something useful for the report. The key is the token, this
/// processor should replace in the report template file.
///
class IPluginDataProcessor {
 public:
  ///
  /// \brief The data processor type
  /// \details A function that processes the plugin data. This is bound to
  /// actual records later on.
  ///
  using DataProcessor =
      std::function<std::string(utility::datastructure::Table)>;

  ///
  /// \brief ~IPluginDataProcessor
  ///
  virtual ~IPluginDataProcessor() = default;

  ///
  /// \brief Get the defined processor
  /// \return The processor function
  ///
  [[nodiscard]] virtual DataProcessor processor() const = 0;

  ///
  /// \brief Get the key, this processor should replace
  /// \return The key in the report
  /// \details Note that keys should have the format \%\%KEY\%\%
  ///
  [[nodiscard]] virtual std::string key() const = 0;
};

}  // namespace imonitorplugin
// LCOV_EXCL_STOP
#endif  // IPLUGINDATAPROCESSOR_H
