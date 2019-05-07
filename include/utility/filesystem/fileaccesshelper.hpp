#ifndef FILEACCESSHELPER_H
#define FILEACCESSHELPER_H

#include <string>
#include <vector>

namespace utility {
namespace filesystem {

///
/// \brief List all files in a directory
/// \details Lists only regular files, not directories or any other sort of
/// directory entries
/// \param path The base path to list files in
/// \return A vector with all files in the directory
///
std::vector<std::string> ListFiles(const std::string& path);

///
/// \brief Build a absolute path using a relative one (does nothing, of the
/// provided path does not start with a dot \param path The relative path to
/// translate \return A absolute path
///
std::string MakeAbsolutePath(const std::string& path);

///
/// \brief Combine arguments to a path
/// \param parts Any number of path elements
/// \return A new path that contains all provided parts
///
std::string PathCombine(const std::vector<std::string>& parts);
}  // namespace filesystem
}  // namespace utility

#endif  // FILEACCESSHELPER_H
