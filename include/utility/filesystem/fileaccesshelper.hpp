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
}  // namespace filesystem
}  // namespace utility

#endif  // FILEACCESSHELPER_H
