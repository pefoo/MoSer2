#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include "catch2/catch.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"

std::string GetsinxScript() {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << "plot_sinx.gp";
  return ss.str();
}

std::string GetRecordsScript() {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << "plot_records.gp";
  return ss.str();
}

std::string GetsinxReferenceImageFile() {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << "plot_sinx_reference.png";
  return ss.str();
}

std::string GetsinxReferenceEncodedImageFile() {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << "plot_sinx_encoded_reference";
  return ss.str();
}

std::string GetrecordsReferenceImageFile() {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << "plot_records_reference.png";
  return ss.str();
}

std::string GetrecordsReferenceEncodedImageFile() {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << "plot_records_encoded_reference";
  return ss.str();
}

TEST_CASE("Execute script, pipe to file", "[DataProcessorHelper]") {
  dataprocessorhelper::gnuplot::GnuPlotBackend::instance().set_mock_call(true);
  dataprocessorhelper::gnuplot::GnuPlotParameterDict dict{};
  dict.AddParameter("titletext", "unit test 1", true);

  std::string output_file = std::filesystem::temp_directory_path().string() +
                            "/" + std::to_string(std::time(nullptr));
  REQUIRE(dataprocessorhelper::gnuplot::GnuPlotBackend::instance().Invoke(
              GetsinxScript(), dict, output_file) == 0);

  std::ifstream output_stream{output_file};
  std::string content{std::istreambuf_iterator<char>{output_stream},
                      std::istreambuf_iterator<char>{}};

  REQUIRE(content == "\"plot_sinx.gp\"\n\"titletext='unit test 1'\"\n");
  std::filesystem::remove(output_file);
}

TEST_CASE("Execute script with automatic intermediate file creation",
          "[DataProcessorHelper]") {
  dataprocessorhelper::gnuplot::GnuPlotBackend::instance().set_mock_call(true);
  dataprocessorhelper::gnuplot::GnuPlotParameterDict dict{};
  utility::datastructure::Table d{"sample data"};
  d.AddColumn(utility::datastructure::DataColumn<int>("c_int", {17, 42, 17}));
  d.AddColumn(utility::datastructure::DataColumn<int>(
      "timestamp", {1558784370, 1558784375, 1558784380}));
  dict.AddParameter("y_series_count", "1", false);

  auto output_file = utility::filesystem::GetTempFile();
  REQUIRE(dataprocessorhelper::gnuplot::ExecuteScript(GetRecordsScript(), d,
                                                      *output_file, dict) == 0);
  std::ifstream output_stream{*output_file};
  std::string content{std::istreambuf_iterator<char>{output_stream},
                      std::istreambuf_iterator<char>{}};

  REQUIRE(content == "\"plot_records.gp\"\n\"y_series_count=1\"\n");
}

TEST_CASE(
    "Execute script with auto intermediate file creation and convert output to "
    "base64",
    "[DataProcessorHelper]") {
  dataprocessorhelper::gnuplot::GnuPlotBackend::instance().set_mock_call(true);
  utility::datastructure::Table d{"sample data"};
  d.AddColumn(utility::datastructure::DataColumn<int>("c_int", {17, 42, 17}));
  d.AddColumn(utility::datastructure::DataColumn<int>(
      "timestamp", {1558784370, 1558784375, 1558784380}));
  dataprocessorhelper::gnuplot::GnuPlotParameterDict dict{};
  dict.AddParameter("y_series_count", "1", false);

  auto encoded_img = dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
      GetRecordsScript(), d, dict);
  std::ifstream ref_stream{GetrecordsReferenceEncodedImageFile()};
  std::string ref_content;
  std::getline(ref_stream, ref_content);
  REQUIRE(encoded_img ==
          "InBsb3RfcmVjb3Jkcy5ncCIKInlfc2VyaWVzX2NvdW50PTEiCg==");
}

TEST_CASE("Execute script and convert output to base64",
          "[DataProcessorHelper]") {
  dataprocessorhelper::gnuplot::GnuPlotBackend::instance().set_mock_call(true);
  dataprocessorhelper::gnuplot::GnuPlotParameterDict dict{};
  dict.AddParameter("titletext", "unit test 1", true);

  auto encoed_img = dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
      GetsinxScript(), dict);

  std::ifstream ref_stream{GetsinxReferenceEncodedImageFile()};
  std::string ref_content;
  std::getline(ref_stream, ref_content);
  REQUIRE(encoed_img ==
          "InBsb3Rfc2lueC5ncCIKInRpdGxldGV4dD0ndW5pdCB0ZXN0IDEnIgo=");
}
