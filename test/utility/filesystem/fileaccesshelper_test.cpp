#include "utility/filesystem/fileaccesshelper.hpp"
#include <sstream>
#include "catch2/catch.hpp"

// Test the file exists functionality
TEST_CASE("File exists", "[utility]") {
  REQUIRE(utility::filesystem::FileExists("/proc/stat"));
  REQUIRE_FALSE(utility::filesystem::FileExists("foobar/slknse"));
}

TEST_CASE("Path combine", "[utility]") {
  auto path =
      utility::filesystem::PathCombine({"/", "foobar", "/", "bar", "foo.txt"});
  REQUIRE(path == "/foobar/bar/foo.txt");
}
