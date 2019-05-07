#include "utility/filesystem/fileaccesshelper.hpp"
#include <sstream>
#include "catch2/catch.hpp"

TEST_CASE("Path combine", "[utility]") {
  auto path =
      utility::filesystem::PathCombine({"/", "foobar", "/", "bar", "foo.txt"});
  REQUIRE(path == "/foobar/bar/foo.txt");
}
