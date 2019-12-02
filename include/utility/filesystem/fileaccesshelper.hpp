#ifndef FILEACCESSHELPER_H
#define FILEACCESSHELPER_H

#include <filesystem>
#include <memory>
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

///
/// \brief Create a temporary file name.
/// \details This function is using the <a
/// href="https://linux.die.net/man/3/mkstemp">POSIX defined function
/// mkstemp</a> to generate a unique temporary file name.
/// The generated file is closed immediately. (see POSIX standard docs)
/// \note mkstemp ensures the file name is unique. This does NOT hold true,
/// if you add a path to the file.
/// \param prefix The prefix for the new file. The result will look like
/// prefix_{random stuff}
/// \return The temp file name. Empty string if mkstemp fails
///
std::string GetTempFileName(const std::string& prefix = "moser2_tmp_file");

///
/// \brief Get a temporary file that is removed once the shared pointer calls
/// its internal destructor
/// \details See utility::filesystem::GetTempFileName
/// for a description of the generated file name.
/// \param prefix The prefix for the new file.
/// \return A shared pointer that stores the file name
///
std::shared_ptr<std::string> GetTempFile(
    const std::string& prefix = "moser2_tmp_file");

}  // namespace filesystem
}  // namespace utility

#endif  // FILEACCESSHELPER_H
