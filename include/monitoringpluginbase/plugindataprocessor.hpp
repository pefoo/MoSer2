#ifndef PLUGINDATAPROCESSOR_H
#define PLUGINDATAPROCESSOR_H

#include <imonitoringplugin/iplugindataprocessor.hpp>
#include <string>

namespace monitoringpluginbase {

///
/// \brief The imonitorplugin::IPluginDataProcessor implementation
///
class PluginDataProcessor : public imonitorplugin::IPluginDataProcessor {
 public:
  ///
  /// \brief Create a new instance
  /// \param token The token, this processor will replace
  /// \param processor The processor
  ///
  PluginDataProcessor(
      std::string token,
      imonitorplugin::IPluginDataProcessor::DataProcessor processor);

  ///
  /// \copydoc imonitorplugin::IPluginDataProcessor::processor()
  ///
  [[nodiscard]] DataProcessor processor() const override;

  ///
  /// \copydoc imonitorplugin::IPluginDataProcessor::key()
  ///
  [[nodiscard]] std::string key() const override;

 private:
  const std::string token_;
  imonitorplugin::IPluginDataProcessor::DataProcessor processor_;
};
}  // namespace monitoringpluginbase

#endif  // PLUGINDATAPROCESSOR_H
