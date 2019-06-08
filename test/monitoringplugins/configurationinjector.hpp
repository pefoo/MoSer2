#ifndef CONFIGURATIONINJECTOR_HPP
#define CONFIGURATIONINJECTOR_HPP

#include <string>

///
/// \brief A helper class that injects a config file from test/testdata
/// \note The old configuration file is restored upon destruction of this class
///
class ConfigurationInjector
{
public:
  ConfigurationInjector(std::string plugin_name);
  ~ConfigurationInjector();
private:
  const std::string plugin_name_;
  std::string config_file_;
  std::string config_backup_;
};

#endif // CONFIGURATIONINJECTOR_HPP
