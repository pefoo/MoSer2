#include "utility/filesystem/fileaccesshelper.hpp"
#include <libgen.h>
#include <linux/limits.h>
#include <unistd.h>
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

std::filesystem::__cxx11::path utility::filesystem::GetCurrentExecutablePath() {
  char path[PATH_MAX];
  auto c = readlink("/proc/self/exe", path, PATH_MAX);
  return std::filesystem::canonical(dirname(path));
}

std::filesystem::__cxx11::path
utility::filesystem::MakeAbsolutePathFromExecutable(const std::string& path) {
  if (path.at(0) != '.') {
    return path;
  }

  return PathCombine({GetCurrentExecutablePath().string(), path});
}
