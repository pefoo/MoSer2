#include "reporter/templateprocessor/templateprocessor.hpp"
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

TEST_CASE("Tokenprocessor test", "[Reporter]") {
  reporter::templateprocessor::TemplateProcessor processor{Tokens()};

  auto result = processor.ProcessTemplate(GetTestFile("reporter_template"));

  std::ifstream content{result};
  std::stringstream buffer;
  buffer << content.rdbuf();

  std::string expected =
      "this\nis some\nrandom\ntext\nhere is the first key: unit test "
      "_1\n\nnext "
      "line contains the second key\nfoobar\nanother random line\n";
  REQUIRE(buffer.str() == expected);

  std::remove(result.c_str());
}
