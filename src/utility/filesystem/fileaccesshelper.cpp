#include "utility/filesystem/fileaccesshelper.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

std::filesystem::path utility::filesystem::PathCombine(
    const std::vector<std::string>& parts) {
  std::string path;
  const char sep = '/';
  for (const auto& part : parts) {
    if (!path.empty() && part.front() == sep) {
      path += part.substr(1, part.size() - 1);
    } else {
      path += part;
    }
    if (path.back() != sep) {
      path += sep;
    }
  }
  // Return path without the last slash
  return std::filesystem::path(path.substr(0, path.size() - 1));
}
