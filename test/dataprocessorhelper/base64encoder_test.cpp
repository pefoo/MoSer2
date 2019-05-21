#include <filesystem>
#include <fstream>
#include <sstream>
#include "catch2/catch.hpp"
#include "dataprocessorhelper/base64.hpp"

TEST_CASE("Encode base 64", "[DataProcessorHelper]") {
  SECTION("Null byte and empty strings are treated equal") {
    REQUIRE(dataprocessorhelper::EncodeBase64("") == "");
    REQUIRE(dataprocessorhelper::EncodeBase64("\0\0") == "");
  }

  SECTION("Padding is correct") {
    REQUIRE(dataprocessorhelper::EncodeBase64("f") == "Zg==");
    REQUIRE(dataprocessorhelper::EncodeBase64("fo") == "Zm8=");
    REQUIRE(dataprocessorhelper::EncodeBase64("foo") == "Zm9v");
    REQUIRE(dataprocessorhelper::EncodeBase64("foob") == "Zm9vYg==");
    REQUIRE(dataprocessorhelper::EncodeBase64("fooba") == "Zm9vYmE=");
    REQUIRE(dataprocessorhelper::EncodeBase64("foobar") == "Zm9vYmFy");
  }

  SECTION("Special character are handled") {
    REQUIRE(dataprocessorhelper::EncodeBase64("foo\nbar") == "Zm9vCmJhcg==");
    REQUIRE(dataprocessorhelper::EncodeBase64("foo\tbar") == "Zm9vCWJhcg==");
  }
}
