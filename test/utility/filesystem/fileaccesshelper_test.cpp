#include "utility/filesystem/fileaccesshelper.hpp"
#include <fstream>
#include <sstream>
#include "catch2/catch.hpp"

TEST_CASE("Path combine", "[utility]") {
  auto path =
      utility::filesystem::PathCombine({"/", "foobar", "/", "bar", "foo.txt"});
  REQUIRE(path == "/foobar/bar/foo.txt");
}

TEST_CASE("Current path", "[utility]") {
  auto path = utility::filesystem::GetCurrentExecutablePath();
  REQUIRE(path == LIBRARY_DIR);
}

TEST_CASE("Make absolute path", "[utility]") {
  auto untouched =
      utility::filesystem::MakeAbsolutePathFromExecutable("/.foobar");
  REQUIRE(untouched == "/.foobar");

  auto path = utility::filesystem::MakeAbsolutePathFromExecutable("foobar");
  REQUIRE(path == std::string(LIBRARY_DIR) + "/foobar");
}

TEST_CASE("Get tmp file", "[utility]") {
  std::string file_name;
  {
    auto f = utility::filesystem::GetTempFile();
    file_name = *f;
    std::ofstream s{*f};
    s << "test";
    s.close();
    REQUIRE(std::filesystem::exists(*f));
  }
  // Smart pointer should have called delete file
  REQUIRE(!std::filesystem::exists(file_name));
}
