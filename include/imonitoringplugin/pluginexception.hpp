#ifndef PLUGINEXCEPTION_H
#define PLUGINEXCEPTION_H

#include <cstring>
#include <exception>
#include <string>
#include <utility>

namespace imonitorplugin {

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
  PluginException(std::string plugin_name, std::string msg)
      : plugin_name_(std::move(plugin_name)),
        msg_(std::move(msg)),
        what_("Plugin: " + this->plugin_name_ + " Error: " + this->msg_) {}

  ///
  /// \brief ~PluginException
  ///
  ~PluginException() noexcept override = default;

  ///
  /// \brief The exception message including the plugin name
  /// \return A c style string with the exception message
  ///
  const char* what() const noexcept override { return this->what_.c_str(); }

  ///
  /// \brief Get the plugin name
  /// \return The name of the plugin
  ///
  std::string plugin_name() const { return plugin_name_; }

  ///
  /// \brief Get the thrown message
  /// \return The message
  ///
  std::string msg() const { return msg_; }

 private:
  std::string plugin_name_;
  std::string msg_;
  std::string what_;
};
}  // namespace imonitorplugin

#endif  // PLUGINEXCEPTION_H
