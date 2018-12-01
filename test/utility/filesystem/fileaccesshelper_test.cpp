#include "utility/filesystem/fileaccesshelper.hpp"
#include <sstream>
#include "catch2/catch.hpp"

// Test the file exists functionality
TEST_CASE("File access helper", "[utility]") {
  REQUIRE(utility::filesystem::FileExists("/proc/stat"));
  REQUIRE_FALSE(utility::filesystem::FileExists("foobar/slknse"));
}
