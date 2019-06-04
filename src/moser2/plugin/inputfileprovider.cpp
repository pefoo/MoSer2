#include "inputfileprovider.hpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include "imonitoringplugin/inputfilecontent.hpp"

moser2::plugin::InputFileProvider::InputFileProvider() {}

void moser2::plugin::InputFileProvider::UpdateFiles() {
  for (auto const &[plugin, files] : this->files_) {
    for (auto const &file : files) {
      std::ifstream content{file};
      std::stringstream buffer;
      buffer << content.rdbuf();
      this->file_contents_[plugin][file] =
          imonitorplugin::InputFileContent{std::time(nullptr), buffer.str()};
    }
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));
  for (auto const &[plugin, files] : this->files_) {
    for (auto const &file : files) {
      std::ifstream content{file};
      std::stringstream buffer;
      buffer << content.rdbuf();
      this->file_contents_[plugin][file].setSnapshot_2(buffer.str());
      this->file_contents_[plugin][file].setTimestamp_2(std::time(nullptr));
    }
  }
}

void moser2::plugin::InputFileProvider::RegisterPluginFiles(
    const std::string &plugin, std::vector<std::string> files) {
  if (this->files_[plugin].empty()) {
    this->files_[plugin] = files;
  } else {
    this->files_[plugin].insert(this->files_[plugin].end(), files.begin(),
                                files.end());
  }
}

std::unordered_map<std::string, imonitorplugin::InputFileContent>
moser2::plugin::InputFileProvider::GetFiles(const std::string &plugin) {
  if (this->file_contents_.count(plugin)) {
    return this->file_contents_[plugin];
  }
  return std::unordered_map<std::string, imonitorplugin::InputFileContent>{};
}
