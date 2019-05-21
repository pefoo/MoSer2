#include "dataprocessorhelper/gnuplot/gnuplotwrapper.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include "catch2/catch.hpp"
#include "dataprocessorhelper/gnuplot/gnuplotparameterdict.hpp"

std::string GetScript() {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << "plot_sinx.gp";
  return ss.str();
}

std::string GetReferenceImageFile() {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << "plot_sinx_reference.png";
  return ss.str();
}

std::string GetReferenceEncodedImageFile() {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << "plot_sinx_encoded_reference";
  return ss.str();
}

TEST_CASE("Execute script, pipe to file", "[DataProcessorHelper]") {
  dataprocessorhelper::gnuplot::GnuPlotParameterDict dict{};
  dict.AddParameter("titletext", "unit test 1", true);

  std::string output_file = std::filesystem::temp_directory_path().string() +
                            "/" + std::to_string(std::time(nullptr));
  REQUIRE(dataprocessorhelper::gnuplot::ExecuteScript(GetScript(), output_file,
                                                      dict) == 0);

  std::ifstream output_stream{output_file};
  std::string content{std::istreambuf_iterator<char>{output_stream},
                      std::istreambuf_iterator<char>{}};

  std::ifstream ref_stream{GetReferenceImageFile()};
  std::string ref_content{std::istreambuf_iterator<char>{ref_stream},
                          std::istreambuf_iterator<char>{}};

  REQUIRE(content == ref_content);
  std::filesystem::remove(output_file);
}

TEST_CASE("Execute script and convert output to base64",
          "[DataProcessorHelper]") {
  dataprocessorhelper::gnuplot::GnuPlotParameterDict dict{};
  dict.AddParameter("titletext", "unit test 1", true);

  auto encoed_img = dataprocessorhelper::gnuplot::EncodeScriptOutputToBase64(
      GetScript(), dict);

  std::ofstream out{"/home/peepe/tmp/result"};
  out << encoed_img;

  std::ifstream ref_stream{GetReferenceEncodedImageFile()};
  std::string ref_content;
  std::getline(ref_stream, ref_content);
  REQUIRE(encoed_img == ref_content);
}
