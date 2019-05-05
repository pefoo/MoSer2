#ifndef PLUGINEXCEPTION_H
#define PLUGINEXCEPTION_H

#include <exception>
#include <string>

namespace monitoringpluginbase {

///
/// \brief The plugin exception class
///
class PluginException : public std::exception {
 public:
  ///
  /// \brief Create new exception
  /// \param plugin_name The name of the plugin
  /// \param msg The message
  ///
  PluginException(std::string plugin_name, std::string msg);

  ///
  /// \brief ~PluginException
  ///
  virtual ~PluginException() noexcept override = default;

  ///
  /// \brief The exception message including the plugin name
  /// \return A c style string with the exception message
  ///
  virtual const char *what() const noexcept override;

  ///
  /// \brief Get the plugin name
  /// \return The name of the plugin
  ///
  std::string plugin_name() const;

  ///
  /// \brief Get the thrown message
  /// \return The message
  ///
  std::string msg() const;

 private:
  std::string plugin_name_;
  std::string msg_;
};
}  // namespace monitoringpluginbase

#endif  // PLUGINEXCEPTION_H
