#include "utility/filesystem/fileaccesshelper.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

bool utility::filesystem::FileExists(const std::string& name) {
  struct stat buffer {};
  return (stat(name.c_str(), &buffer) == 0);
}

std::string utility::filesystem::GetCurrentWorkingDir() {
  char cwd[FILENAME_MAX];
  getcwd(cwd, FILENAME_MAX);
  return std::string(cwd);
}

std::vector<std::string> utility::filesystem::ListFiles(
    const std::string& path) {
  DIR* dir;
  dirent* ent;
  std::vector<std::string> files;

  if ((dir = opendir(path.c_str())) != nullptr) {
    while ((ent = readdir(dir)) != nullptr) {
      if (ent->d_type == DT_REG) {
        files.push_back(std::string(ent->d_name));
      }
    }
  } else {
    throw std::runtime_error("Failed to open the directory " + path);
  }
  return files;
}

std::string utility::filesystem::MakeAbsolutePath(const std::string& path) {
  if (path.size() == 0) {
    return "";
  }
  if (path.at(0) == '.') {
    return GetCurrentWorkingDir() + path.substr(1, path.size() - 1);
  }
  return path;
}
