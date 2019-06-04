#ifndef INPUTFILEPROVIDER_HPP
#define INPUTFILEPROVIDER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include "imonitoringplugin/inputfilecontent.hpp"

namespace moser2 {
namespace plugin {

///
/// \brief The input file provider
/// \details Reads files and stores snapshots of them
///
class InputFileProvider {
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
  /// \brief Register plugin input files
  /// \param plugin The plugin to read the file(s) for
  /// \param files The file(s) to register
  ///
  void RegisterPluginFiles(const std::string& plugin,
                           std::vector<std::string> files);

  ///
  /// \brief Get the file content for a plugin
  /// \param plugin The name of the plugin
  /// \return All registered files, with 2 snapshots each
  ///
  std::unordered_map<std::string, imonitorplugin::InputFileContent> GetFiles(
      const std::string& plugin);

 private:
  // Key: plugin name. value: vector of registeres files
  std::unordered_map<std::string, std::vector<std::string>> files_;
  // Key: plugin name. value: map with file name and content
  std::unordered_map<
      std::string,
      std::unordered_map<std::string, imonitorplugin::InputFileContent>>
      file_contents_;
};

}  // namespace plugin
}  // namespace moser2

#endif  // INPUTFILEPROVIDER_HPP
