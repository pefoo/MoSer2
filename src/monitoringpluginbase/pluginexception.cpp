#include "monitoringpluginbase/pluginexception.hpp"
#include <utility>

monitoringpluginbase::PluginException::PluginException(std::string plugin_name,
                                                       std::string msg)
    : plugin_name_(std::move(plugin_name)), msg_(std::move(msg)) {}

const char *monitoringpluginbase::PluginException::what() const noexcept {
  return std::string("Plugin: " + this->plugin_name_ + ". Error: " + this->msg_)
      .c_str();
}

std::string monitoringpluginbase::PluginException::msg() const { return msg_; }

std::string monitoringpluginbase::PluginException::plugin_name() const {
  return plugin_name_;
}
