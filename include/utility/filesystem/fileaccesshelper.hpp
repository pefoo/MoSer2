#ifndef FILEACCESSHELPER_H
#define FILEACCESSHELPER_H

#include <filesystem>
#include <string>
#include <vector>

namespace utility {
namespace filesystem {

///
/// \brief Combine arguments to a path
/// \param parts Any number of path elements
/// \return A new path that contains all provided parts
///
std::filesystem::path PathCombine(const std::vector<std::string>& parts);

///
/// \brief Get the path to the current executable
/// \return A c++17 path
///
std::filesystem::path GetCurrentExecutablePath();

///
/// \brief Translate a (possible) relative path to an absolute one using the
/// current executable path as base
/// \note Does nothing, if path does not start with a dot
/// \param path The (possible) relative path.
///
std::filesystem::path MakeAbsolutePathFromExecutable(const std::string& path);

}  // namespace filesystem
}  // namespace utility

#endif  // FILEACCESSHELPER_H
