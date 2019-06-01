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
#include "utility/datastructure/anyhelper.hpp"

// Custom deleter for smart pointer, that holds a temporary file (name)
void DeleteFile(std::string *);

// Write the intermediate data file
std::unique_ptr<std::string, decltype(&DeleteFile)> WriteIntermediateFile(
    std::vector<imonitorplugin::PluginData> records,
    std::vector<std::string> column_names, const std::string &data_separator,
    const std::string time_format,
    std::function<bool(const std::string &)> record_filter =
        [](const std::string &) { return true; });

std::string MakeTmpFileName();

int dataprocessorhelper::gnuplot::ExecuteScript(
    const std::string &script, const std::string &output_file,
    const dataprocessorhelper::gnuplot::GnuPlotParameterDict &parameter) {
  auto call =
      "gnuplot -e " + parameter.ToString() + " " + script + " > " + output_file;
  return std::system(call.c_str());
}

std::string dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
    const std::string &script,
    const dataprocessorhelper::gnuplot::GnuPlotParameterDict &parameter) {
  // Even in case anything goes wrong along the way, this will delete the
  // temporary file
  std::unique_ptr<std::string, decltype(&DeleteFile)> tmp_file(
      new std::string(MakeTmpFileName()), DeleteFile);
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

int dataprocessorhelper::gnuplot::ExecuteScript(
    const std::string &script, std::vector<imonitorplugin::PluginData> records,
    const std::string &data_file_var_name, const std::string &output_file,
    dataprocessorhelper::gnuplot::GnuPlotParameterDict parameter,
    const std::string &data_separator, std::vector<std::string> column_names,
    const std::string &time_format) {
  if (column_names.size() == 0) {
    column_names.push_back("Timestamp");
    for (auto const &data : records.front().data()) {
      column_names.push_back(data.first);
    }
  }
  auto tmp_file =
      WriteIntermediateFile(records, column_names, data_separator, time_format);
  parameter.AddParameter(data_file_var_name, *tmp_file, true, true);
  return ExecuteScript(script, output_file, parameter);
}

std::string dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
    const std::string &script, std::vector<imonitorplugin::PluginData> records,
    const std::string &data_file_var_name,
    dataprocessorhelper::gnuplot::GnuPlotParameterDict parameter,
    const std::string &data_separator,
    std::function<bool(const std::string &)> record_filter,
    std::vector<std::string> column_names, const std::string &time_format) {
  if (column_names.size() == 0) {
    column_names.push_back("Timestamp");
    for (auto const &data : records.front().data()) {
      if (record_filter(data.first)) {
        column_names.push_back(data.first);
      }
    }
  }
  auto tmp_file = WriteIntermediateFile(records, column_names, data_separator,
                                        time_format, record_filter);
  parameter.AddParameter(data_file_var_name, *tmp_file, true, true);
  return EncodeScriptOutputToBase64(script, parameter);
}

void DeleteFile(std::string *file) {
  if (std::filesystem::exists(*file)) {
    std::filesystem::remove(*file);
  }
}

std::unique_ptr<std::string, decltype(&DeleteFile)> WriteIntermediateFile(
    std::vector<imonitorplugin::PluginData> records,
    std::vector<std::string> column_names, const std::string &data_separator,
    const std::string time_format,
    std::function<bool(const std::string &)> record_filter) {
  //  if (column_names.size() != records.front().data().size() + 1) {
  //    throw std::runtime_error(
  //        "The number of provided column names does not match the number of "
  //        "records for the intermediate data file.");
  //  }

  std::unique_ptr<std::string, decltype(&DeleteFile)> tmp_file(
      new std::string(MakeTmpFileName()), DeleteFile);

  std::ofstream data_stream{*tmp_file};
  for (auto const &header : column_names) {
    data_stream << header << data_separator;
  }
  data_stream.seekp(-1, std::ios_base::end);
  data_stream << std::endl;

  for (size_t i = 0; i < records.size(); i++) {
    time_t t = records.at(i).timestamp();
    struct tm *tm = new struct tm;
    tm = localtime_r(&t, tm);
    data_stream << std::put_time(tm, time_format.c_str());

    for (const auto &data : records.at(i).data()) {
      if (record_filter(data.first)) {
        data_stream << data_separator
                    << utility::datastructure::PrimitiveAnyToString(
                           data.second);
      }
    }
    data_stream << std::endl;
  }
  data_stream.flush();
  return tmp_file;
}

std::string MakeTmpFileName() {
  static std::mt19937 gen{std::random_device()()};
  static std::uniform_int_distribution<> dist{0,
                                              std::numeric_limits<int>::max()};
  auto file_name = std::to_string(dist(gen));
  return std::filesystem::temp_directory_path().string() + "/" + file_name;
}
