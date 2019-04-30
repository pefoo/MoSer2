#include "utility/filesystem/fileaccesshelper.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <string>

bool utility::filesystem::FileExists(const std::string& name) {
  struct stat buffer {};
  return (stat(name.c_str(), &buffer) == 0);
}

std::string utility::filesystem::GetCurrentWorkingDir() {
  char cwd[FILENAME_MAX];
  getcwd(cwd, FILENAME_MAX);
  return std::string(cwd);
}
