#include "utility/datastructure/tablefilter.hpp"
#include <vector>
#include "catch2/catch.hpp"

TEST_CASE("Column filter", "[Utility]") {
  auto multi_column_filter =
      utility::datastructure::CreateColumnFilter({"c1", "c2"});
  REQUIRE(multi_column_filter("c1"));
  REQUIRE(multi_column_filter("c2"));
  REQUIRE(!multi_column_filter("c12"));
}

TEST_CASE("Field filter", "[Utility]") {
  auto field_filter =
      utility::datastructure::CreateFieldFilter({{"c1", "v1"}, {"c2", "v2"}});
  REQUIRE(field_filter("c1", "v1"));
  REQUIRE(field_filter("c2", "v2"));
  REQUIRE(!field_filter("c1", "v2"));
  REQUIRE(!field_filter("c2", "v1"));
}
