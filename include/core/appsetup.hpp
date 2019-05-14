#ifndef APPSETUP_H
#define APPSETUP_H

#include <string>

namespace core {

///
/// \brief Get the application config file
/// \return The absolute path to the configuration file
///
std::string GetApplicationConfigFile();

///
/// \brief Configure the logger using the default configuration file
///
void ConfigureLogger();

} // namespace core

#endif //APPSETUP_H
