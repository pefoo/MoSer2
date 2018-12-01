#ifndef FILEACCESSHELPER_H
#define FILEACCESSHELPER_H

#include <string>

namespace utility {
namespace filesystem {

///
/// \brief Check if the file exists
/// \return True, if the file exists
///
bool FileExists(const std::string&);

}  // namespace filesystem
}  // namespace utility

#endif  // FILEACCESSHELPER_H
