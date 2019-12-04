#include "utility/helper/stringhelper.hpp"
#include <string>
#include "catch2/catch.hpp"

TEST_CASE("String starts with", "[utility]") {
  std::string a = "foo bat";
  REQUIRE(utility::helper::StringStartsWith(a, "foo"));
  REQUIRE(!utility::helper::StringStartsWith(a, "bat"));
}

TEST_CASE("String rgx grep", "[utility]") {
  std::string a = "foo bat 17 bar";
  REQUIRE(
      utility::helper::StringRgxGrep(a, R"(^fo{2}\sb.t\s\d{2}\s[rba]{3}$)"));
  std::smatch sm;
  REQUIRE(
      utility::helper::StringRgxGrep(a, R"(^[\w\s]+(\d{2})\s(\w{3})$)", &sm));
  // sm contains the full match and the two groups (thus 3 elements)
  REQUIRE(sm.size() == 3);
  REQUIRE(sm[1] == "17");
  REQUIRE(sm[2] == "bar");
}
