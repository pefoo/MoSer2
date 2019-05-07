#include "utility/filesystem/fileaccesshelper.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

std::string utility::filesystem::MakeAbsolutePath(const std::string& path) {
  throw std::runtime_error("Not supported anymore");
  if (path.empty()) {
    return "";
  }
  if (path.at(0) == '.') {
    return path.substr(1, path.size() - 1);
  }
  return path;
}

std::string utility::filesystem::PathCombine(
    const std::vector<std::string>& parts) {
  std::string path;
  for (const auto& part : parts) {
    if (!path.empty() && part.front() == '/') {
      path += part.substr(1, part.size() - 1);
    } else {
      path += part;
    }
    if (path.back() != '/') {
      path += "/";
    }
  }
  // Return path without the last slash
  return path.substr(0, path.size() - 1);
}
