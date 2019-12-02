#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include "dataprocessorhelper/base64.hpp"
#include "filesystem"
#include "utility/filesystem/fileaccesshelper.hpp"

/*
 *  gnuplot backend
 */

dataprocessorhelper::gnuplot::GnuPlotBackend &
dataprocessorhelper::gnuplot::GnuPlotBackend::instance() {
  static GnuPlotBackend gpl_backend_;
  return gpl_backend_;
}
dataprocessorhelper::gnuplot::GnuPlotBackend::GnuPlotBackend()
    : mock_call_(false), settings_(std::make_shared<GnuPlotSettings>()) {}

bool dataprocessorhelper::gnuplot::GnuPlotBackend::mock_call() const {
  return mock_call_;
}

void dataprocessorhelper::gnuplot::GnuPlotBackend::set_mock_call(
    bool mock_call) {
  mock_call_ = mock_call;
}

int dataprocessorhelper::gnuplot::GnuPlotBackend::Invoke(
    const std::string &script,
    const dataprocessorhelper::gnuplot::GnuPlotParameterDict &params,
    const std::string &out_file) {
  if (this->mock_call()) {
    std::ofstream f{out_file};
    f << std::filesystem::path{script}.filename() << std::endl;
    f << params.ToString() << std::endl;
    return 0;
  }
  auto call =
      "gnuplot -e " + params.ToString() + " " + script + " > " + out_file;

  return std::system(call.c_str());
}

std::shared_ptr<dataprocessorhelper::gnuplot::GnuPlotSettings>
dataprocessorhelper::gnuplot::GnuPlotBackend::settings() {
  return this->settings_;
}

/*
 * gnuplot wrapper
 */

std::string dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
    const std::string &script,
    const dataprocessorhelper::gnuplot::GnuPlotParameterDict &parameter) {
  auto tmp_file = utility::filesystem::GetTempFile();
  if (GnuPlotBackend::instance().Invoke(script, parameter, *tmp_file)) {
    throw std::runtime_error("Failed to execute the script " + script);
  }

  std::ifstream output_stream{*tmp_file};
  std::string content{std::istreambuf_iterator<char>{output_stream},
                      std::istreambuf_iterator<char>{}};

  return dataprocessorhelper::EncodeBase64(content);
}

int dataprocessorhelper::gnuplot::ExecuteScript(
    const std::string &script, const utility::datastructure::Table &data,
    const std::string &output_file,
    dataprocessorhelper::gnuplot::GnuPlotParameterDict parameter,
    const std::function<bool(const std::string &)> filter) {
  auto tmp_file = utility::filesystem::GetTempFile();
  data.ToFile(GnuPlotBackend::instance().settings()->data_file_separator,
              *tmp_file, true, filter);
  // Do not add the tmp file path to the params for mock mode (since the file
  // path is random)
  if (!GnuPlotBackend::instance().mock_call()) {
    parameter.AddParameter(
        GnuPlotBackend::instance().settings()->data_file_variable_name,
        *tmp_file, true, true);
  }
  return GnuPlotBackend::instance().Invoke(script, parameter, output_file);
}

std::string dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
    const std::string &script, utility::datastructure::Table data,
    dataprocessorhelper::gnuplot::GnuPlotParameterDict parameter,
    const std::function<bool(const std::string &)> filter) {
  auto tmp_file = utility::filesystem::GetTempFile();
  data.ToFile(GnuPlotBackend::instance().settings()->data_file_separator,
              *tmp_file, true, filter);
  // Do not add the tmp file path to the params for mock mode (since the file
  // path is random)
  if (!GnuPlotBackend::instance().mock_call()) {
    parameter.AddParameter(
        GnuPlotBackend::instance().settings()->data_file_variable_name,
        *tmp_file, true, true);
  }
  return EncodeScriptOutputToBase64(script, parameter);
}
