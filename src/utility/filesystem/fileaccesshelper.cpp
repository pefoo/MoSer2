#include "utility/filesystem/fileaccesshelper.hpp"
#include <libgen.h>
#include <linux/limits.h>
#include <unistd.h>
#include <cstring>
#include <filesystem>
#include <memory>
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
  readlink("/proc/self/exe", path, PATH_MAX);
  return std::filesystem::canonical(dirname(path));
}

std::filesystem::__cxx11::path
utility::filesystem::MakeAbsolutePathFromExecutable(const std::string& path) {
  if (path.at(0) == '/') {
    return path;
  }

  return PathCombine({GetCurrentExecutablePath().string(), path});
}

std::string utility::filesystem::GetTempFileName(const std::string& prefix) {
  const std::string tmp_dir = std::filesystem::temp_directory_path();
  constexpr char placeholder[] = "_XXXXXX";
  // we need space for the prefix, the placeholder required my mkstemp and the
  // null terminator
  auto name_template = std::unique_ptr<char[]>(
      new char[tmp_dir.size() + prefix.size() + std::strlen(placeholder) + 1]);
  std::strcpy(name_template.get(), prefix.c_str());
  std::strcat(name_template.get(), placeholder);
  int df = mkstemp(name_template.get());
  if (df < 0) {
    return "";
  }
  close(df);
  return std::string{name_template.get()};
}

std::shared_ptr<std::string> utility::filesystem::GetTempFile(
    const std::string& prefix) {
  auto deleter = [](const std::string* file) {
    // Delete the file
    if (std::filesystem::exists(*file)) {
      std::filesystem::remove(*file);
    }
    // Free memory
    delete file;
  };
  return std::shared_ptr<std::string>(new std::string(GetTempFileName(prefix)),
                                      deleter);
}
