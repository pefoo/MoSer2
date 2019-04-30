#ifndef FILEACCESSHELPER_H
#define FILEACCESSHELPER_H

#include <string>
#include <vector>

namespace utility {
namespace filesystem {

///
/// \brief Check if the file exists
/// \param path The file path to check
/// \return True, if the file exists
///
bool FileExists(const std::string& path);

///
/// \brief Get current working directory
/// \return The current working directory
///
std::string GetCurrentWorkingDir();

///
/// \brief List all files in a directory
/// \details Lists only regular files, not directories or any other sort of
/// directory entries
/// \param path The base path to list files in
/// \return A vector with all files in the directory
///
std::vector<std::string> ListFiles(const std::string& path);

}  // namespace filesystem
}  // namespace utility

#endif  // FILEACCESSHELPER_H
