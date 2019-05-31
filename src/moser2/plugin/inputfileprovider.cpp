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
  for (auto const &[plugin, file] : this->files_) {
    std::ifstream content{file};
    std::stringstream buffer;
    buffer << content.rdbuf();
    this->file_contents_[plugin] =
        imonitorplugin::InputFileContent{std::time(nullptr), buffer.str()};
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));
  for (auto const &[plugin, file] : this->files_) {
    std::ifstream content{file};
    std::stringstream buffer;
    buffer << content.rdbuf();
    this->file_contents_[plugin].setSnapshot_2(buffer.str());
  }
}

void moser2::plugin::InputFileProvider::RegisterPluginFile(
    const std::string &plugin, const std::string file) {
  this->files_[plugin] = file;
}

imonitorplugin::InputFileContent moser2::plugin::InputFileProvider::GetFile(
    const std::string &plugin) {
  if (this->file_contents_.count(plugin)) {
    return this->file_contents_[plugin];
  }
  return imonitorplugin::InputFileContent{};
}
