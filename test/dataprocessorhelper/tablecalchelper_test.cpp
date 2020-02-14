#include "dataprocessorhelper/tablecalchelper.hpp"
#include "catch2/catch.hpp"
#include "utility/datastructure/table.hpp"

TEST_CASE("Min value", "[TableCalcHelper]") {
  utility::datastructure::Table t;
  t.AddColumn(utility::datastructure::DataColumn<int>("c1", {10, 20, 30, 40}));
  t.AddColumn(utility::datastructure::DataColumn<int>("c2", {10, 4, 30, 40}));
  t.AddColumn(
      utility::datastructure::DataColumn<int>("timestamp", {-345, 2, 3, 4}));

  REQUIRE(dataprocessorhelper::Min<int>(t) == 4);
}

TEST_CASE("Max value", "[TableCalcHelper]") {
  utility::datastructure::Table t;
  t.AddColumn(utility::datastructure::DataColumn<int>("c1", {10, 20, 30, 40}));
  t.AddColumn(utility::datastructure::DataColumn<int>("c2", {10, 4, 30, 40}));
  t.AddColumn(
      utility::datastructure::DataColumn<int>("timestamp", {-345, 2, 3, 4}));

  REQUIRE(dataprocessorhelper::Max<int>(t) == 40);
}

TEST_CASE("Avg value", "[TableCalcHelper]") {
  utility::datastructure::Table t;
  t.AddColumn(utility::datastructure::DataColumn<int>("c1", {1, 2, 3, 4}));
  t.AddColumn(utility::datastructure::DataColumn<int>("c2", {1, 4, 3, 4}));
  t.AddColumn(
      utility::datastructure::DataColumn<int>("timestamp", {-345, 2, 3, 4}));

  REQUIRE(dataprocessorhelper::Avg<int>(t) == 2.75);
}

TEST_CASE("Filter creation", "[TableCalcHelper]") {
  utility::datastructure::Table t;
  t.AddColumn(utility::datastructure::DataColumn<int>("c1", {1, 2, 3, 4}));
  t.AddColumn(utility::datastructure::DataColumn<int>("c2", {1, 4, 3, 4}));
  t.AddColumn(
      utility::datastructure::DataColumn<int>("timestamp", {-345, 2, 3, 4}));

  auto filter = dataprocessorhelper::Filter::GetFilter({"c1", "c2"});
  auto inclusive_filter =
      dataprocessorhelper::Filter::GetInclusiveFilter({"c1"});
  REQUIRE(!filter("c1"));
  REQUIRE(!filter("c2"));
  REQUIRE(!filter("timestamp"));
  REQUIRE(filter("foobar"));
  REQUIRE(inclusive_filter("c1"));
  REQUIRE(!inclusive_filter("c2"));
  REQUIRE(!inclusive_filter("timestamp"));
}

TEST_CASE("Standard deviation", "[TableCalcHelper]") {
  utility::datastructure::Table t;
  t.AddColumn(utility::datastructure::DataColumn<int>(
      "c1", {9, 2, 5, 4, 12, 7, 8, 11, 9, 3}));
  t.AddColumn(utility::datastructure::DataColumn<int>(
      "c2", {4, 12, 5, 4, 10, 9, 6, 9, 4, 7}));

  auto std_dev = dataprocessorhelper::StdDev<int>(t);
  REQUIRE(std_dev == Approx(2.9832867780352594));
}
