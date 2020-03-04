#include "reporter/templateprocessor/templateprocessor.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include "catch2/catch.hpp"
#include "reporter/templateprocessor/templatetoken.hpp"

std::string GetTestFile(const std::string file_name) {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << file_name;
  return ss.str();
}

std::vector<reporter::templateprocessor::TemplateToken> Tokens() {
  return {reporter::templateprocessor::TemplateToken{
              "key1", []() { return "unit test _1"; }},
          reporter::templateprocessor::TemplateToken{
              "%%foobar%%", []() { return "foobar"; }}};
}

std::string ExpectedContent() {
  return "this\nis some\nrandom\ntext\nhere is the first key: unit test "
         "_1\n\nnext "
         "line contains the second key\nfoobar\nanother random line\n";
}

TEST_CASE("Tokenprocessor normal run", "[Reporter]") {
  reporter::templateprocessor::TemplateProcessor processor{Tokens(), nullptr};

  auto result = processor.ProcessTemplate(GetTestFile("reporter_template"));

  std::ifstream content{result};
  std::stringstream buffer;
  buffer << content.rdbuf();

  REQUIRE(buffer.str() == ExpectedContent());

  std::remove(result.c_str());
}

TEST_CASE("Tokenprocessor in place", "[Reporter]") {
  reporter::templateprocessor::TemplateProcessor processor{Tokens(), nullptr};

  std::filesystem::copy(GetTestFile("reporter_template"), ".tmp",
                        std::filesystem::copy_options::overwrite_existing);
  auto result = processor.ProcessTemplate(".tmp", true);
  REQUIRE(result == ".tmp");

  std::ifstream content{result};
  std::stringstream buffer;
  buffer << content.rdbuf();

  REQUIRE(buffer.str() == ExpectedContent());

  std::remove(result.c_str());
}
