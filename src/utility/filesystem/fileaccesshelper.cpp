#include "utility/filesystem/fileaccesshelper.hpp"
#include <sys/stat.h>
#include <string>

bool utility::filesystem::FileExists(const std::string& name) {
  struct stat buffer {};
  return (stat(name.c_str(), &buffer) == 0);
}
