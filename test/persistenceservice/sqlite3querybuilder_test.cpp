#include <algorithm>
#include <string>
#include "catch2/catch.hpp"
#include "imonitoringplugin/plugindata.hpp"
#include "persistenceservice/sqlite/queryhelper.hpp"

static imonitorplugin::PluginData::data_vector data_vec = {
    {"c_int", utility::datastructure::Any(17)},
    {"c_float", utility::datastructure::Any(17.17f)},
    {"c_double", utility::datastructure::Any(17.17)},
    {"c_int64_t", utility::datastructure::Any(int64_t(17))},
    {"c_string", utility::datastructure::Any(std::string("foobar"))}};
static imonitorplugin::PluginData data("unit_test_plugin", 1546329600,
                                       data_vec);

TEST_CASE("Query helper - create table", "[PersistenceService]") {
  auto query =
      persistenceservice::sqlite::QueryHelper::BuildCreateTableQuery(data);
  std::transform(query.begin(), query.end(), query.begin(), ::toupper);
  REQUIRE(
      query ==
      "CREATE TABLE IF NOT EXISTS UNIT_TEST_PLUGIN(C_INT INTEGER,C_FLOAT "
      "REAL,C_DOUBLE REAL,C_INT64_T INTEGER,C_STRING TEXT,TIMESTAMP INTEGER)");
}

TEST_CASE("Query helper - insert", "[PersistenceService]") {
  auto query = persistenceservice::sqlite::QueryHelper::BuildInsertQuery(data);
  std::transform(query.begin(), query.end(), query.begin(), ::toupper);
  REQUIRE(query ==
          "INSERT INTO UNIT_TEST_PLUGIN "
          "(C_INT,C_FLOAT,C_DOUBLE,C_INT64_T,C_STRING,TIMESTAMP) VALUES "
          "(17,17.170000,17.170000,17,'FOOBAR',1546329600)");
}

TEST_CASE("Query helper - select", "[PersistenceService]") {
  auto query =
      persistenceservice::sqlite::QueryHelper::BuildSelectQuery("foobar", 10);
  std::transform(query.begin(), query.end(), query.begin(), ::toupper);
  REQUIRE(query == "SELECT * FROM FOOBAR WHERE TIMESTAMP > 10");
}
