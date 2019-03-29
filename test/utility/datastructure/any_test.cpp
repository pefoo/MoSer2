#include "utility/datastructure/Any.hpp"
#include "catch2/catch.hpp"

TEST_CASE("Any", "[Utility]") {
  using namespace utility::datastructure;
  Any any_int = Any(5);

  REQUIRE(any_int.type() == typeid(int));
  REQUIRE(any_int.get<int>() == 5);
  REQUIRE_THROWS([&]() { any_int.get<float>(); }());

  Any copied_any_int = any_int;
  REQUIRE(any_int.get<int>() == copied_any_int.get<int>());

  Any moved_any = std::move(copied_any_int);
  REQUIRE(moved_any.get<int>() == 5);
}
