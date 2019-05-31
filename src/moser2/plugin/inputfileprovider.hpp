#ifndef INPUTFILEPROVIDER_HPP
#define INPUTFILEPROVIDER_HPP

#include <string>
#include "imonitoringplugin/inputfilecontent.hpp"
#include <unordered_map>

namespace moser2 {
namespace plugin {

///
/// \brief The input file provider
/// \details Reads files and stores snapshots of them
///
class InputFileProvider
{
public:
  ///
  /// \brief InputFileProvider
  ///
  InputFileProvider();

  ///
  /// \brief Update the snapshots
  ///
  void UpdateFiles();

  ///
  /// \brief Register a new file to read
  /// \param plugin The plugin to read the file for
  /// \param file The file to read
  ///
  void RegisterPluginFile(const std::string& plugin, const std::string file);

  ///
  /// \brief Get the file content for a plugin
  /// \param plugin The name of the plugin
  /// \return Two snapshots of the same file
  ///
  imonitorplugin::InputFileContent GetFile(const std::string& plugin);
private:
  std::unordered_map<std::string, std::string> files_;
  std::unordered_map<std::string, imonitorplugin::InputFileContent> file_contents_;
};

} // namespace plugin
} // namespace moser2

#endif // INPUTFILEPROVIDER_HPP
