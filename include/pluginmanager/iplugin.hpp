#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <string>
namespace pluginmanager {
///
/// \brief The plugin interface
/// \tparam PluginInterface The interface of the wrapped plugin implementation
/// \details Provides access to the underlying plugin which implements
/// PluginInterface
///
template <typename PluginInterface>
class IPlugin {
 public:
  ///
  /// \brief operator ==. Compare the current instance with another one
  /// \param other The other plugin instance
  /// \return True, if the ids are equal
  ///
  virtual bool operator==(const IPlugin* const other) = 0;

  ///
  /// \brief Get the underlying plugin
  /// \return The instance of the actual plugin
  ///
  virtual const PluginInterface* const Instance() const = 0;

  ///
  /// \brief Get the underlying plugin
  /// \return The instance of the actual plugin
  ///
  virtual PluginInterface* const Instance() = 0;

  ///
  /// \brief Get the library path
  /// \return The path to the library the underlying plugin was load from
  ///
  virtual std::string GetLibraryPath() const = 0;
};
}  // namespace pluginmanager
#endif  // IPLUGIN_H
