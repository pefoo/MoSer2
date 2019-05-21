#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include <array>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include "dataprocessorhelper/base64.hpp"

int dataprocessorhelper::gnuplot::ExecuteScript(
    const std::string &script, const std::string &output_file,
    const dataprocessorhelper::gnuplot::GnuPlotParameterDict &parameter) {
  auto call =
      "gnuplot -e " + parameter.ToString() + " " + script + " > " + output_file;
  return std::system(call.c_str());
}

void DeleteFile(std::string *file) {
  if (std::filesystem::exists(*file)) {
    std::filesystem::remove(*file);
  }
}

std::string dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
    const std::string &script,
    const dataprocessorhelper::gnuplot::GnuPlotParameterDict &parameter) {
  // Even in case anything goes wrong along the way, this will delete the
  // temporary file
  std::unique_ptr<std::string, decltype(&DeleteFile)> tmp_file(
      new std::string(std::filesystem::temp_directory_path().string() + "/" +
                      std::to_string(std::time(nullptr))),
      DeleteFile);
  auto call =
      "gnuplot -e " + parameter.ToString() + " " + script + " > " + *tmp_file;

  if (std::system(call.c_str())) {
    throw std::runtime_error("Failed to execute the script " + script);
  }

  std::ifstream output_stream{*tmp_file};
  std::string content{std::istreambuf_iterator<char>{output_stream},
                      std::istreambuf_iterator<char>{}};

  return dataprocessorhelper::EncodeBase64(content);
}
