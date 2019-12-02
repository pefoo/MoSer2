#include "utility/datastructure/table.hpp"
#include <unistd.h>
#include <sstream>
#include "catch2/catch.hpp"
#include "fstream"
#include "utility/datastructure/column.hpp"
#include "utility/datastructure/datacolumn.hpp"

std::string GetTestFile(const std::string file_name) {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << file_name;
  return ss.str();
}

using namespace utility::datastructure;
TEST_CASE("Table", "[Utility]") {
  std::string file = "";
  Table t;

  // Add a predefined column
  std::vector<int> int_vals{0, 1, 2, 3, 4};
  DataColumn<int> int_col{"int_col", int_vals};
  t.AddColumn(int_col);

  // Add implicitly typed column
  std::vector<std::string> string_vals{"v0", "v1", "v2", "v3", "v4"};
  t.AddValue(string_vals[0], "string_col");
  t.AddValue(string_vals[1], "string_col");
  t.AddValue(string_vals[2], "string_col");
  t.AddValue(string_vals[3], "string_col");
  t.AddValue(string_vals[4], "string_col");

  // Add a timestamp column
  t.AddColumn(DataColumn<int>{"timestamp", {9, 8, 7, 6, 5}});

  SECTION("Basic functions") {
    REQUIRE(t.name() == "default");
    t.set_name("test");
    REQUIRE(t.name() == "test");
    REQUIRE(t.MaxSize() == 5);
    REQUIRE(t.IsUniformSize());
    t.AddColumn(DataColumn<int>{"larger_col", {0, 1, 2, 3, 4, 5}});
    REQUIRE(!t.IsUniformSize());
    REQUIRE(t.MaxSize() == 6);
  }

  SECTION("Value access") {
    std::vector<std::string> expected_names{"int_col", "string_col",
                                            "timestamp"};
    REQUIRE_THAT(t.GetColumnNames(),
                 Catch::Matchers::UnorderedEquals(expected_names));

    REQUIRE_THAT(t.GetDataColumn<int>("int_col").data(),
                 Catch::Matchers::Equals(int_vals));
    REQUIRE_THAT(t.GetDataColumn<std::string>("string_col").data(),
                 Catch::Matchers::Equals(string_vals));
  }

  SECTION("Write to file") {
    file = t.ToFile(';', "", [](const std::string& column) {
      return !(column == "larger_col");
    });

    std::fstream ref_stream{GetTestFile("table_file.txt")};
    std::string ref_content((std::istreambuf_iterator<char>(ref_stream)),
                            std::istreambuf_iterator<char>());

    std::fstream actual_stream{file};
    std::string actual_content((std::istreambuf_iterator<char>(actual_stream)),
                               std::istreambuf_iterator<char>());

    REQUIRE(actual_content == ref_content);
  }

  if (access(file.c_str(), F_OK) != -1) {
    remove(file.c_str());
  }
}
