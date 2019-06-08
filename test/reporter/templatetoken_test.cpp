#include "reporter/templateprocessor/templatetoken.hpp"
#include <string>
#include "catch2/catch.hpp"

TEST_CASE("Template token", "[Reporter]") {
  int* v = new int(0);
  reporter::templateprocessor::TemplateToken non_cache_token(
      "t", [&]() { return std::to_string(++(*v)); }, false);

  // Value function is called every time value() is called
  REQUIRE(non_cache_token.value() == "1");
  REQUIRE(non_cache_token.value() == "2");
  REQUIRE(non_cache_token.value() == "3");

  *v = 0;
  reporter::templateprocessor::TemplateToken cache_token(
      "t", [&]() { return std::to_string(++(*v)); });

  // Value function is executed only once
  REQUIRE(cache_token.value() == "1");
  REQUIRE(cache_token.value() == "1");
  REQUIRE(cache_token.value() == "1");

  reporter::templateprocessor::TemplateToken throw_token(
      "t",
      []() -> std::string { throw std::runtime_error("Iam an evil token!"); });

  REQUIRE_NOTHROW(throw_token.value());
  REQUIRE(throw_token.value() == "");
}
