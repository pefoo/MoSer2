#include <unistd.h>
#include <any>
#include <cstdio>
#include <string>
#include "catch2/catch.hpp"
#include "persistenceservice/adapterfactory.hpp"
#include "persistenceservice/adaptersettings.hpp"
#include "persistenceservice/idataadapter.hpp"
#include "persistenceservice/sqlite/sqliteadapter.hpp"
#include "persistenceservice/sqlite/sqlitesettings.hpp"
#include "utility/datastructure/table.hpp"

const std::string database_file = "sqlite3_adapter_test.db";
static imonitorplugin::PluginData::data_vector data_vec = {
    {"c_int", 17},
    {"c_float", 17.17f},
    {"c_double", 17.17},
    {"c_int64_t", int64_t(17)},
    {"c_string", std::string("foobar")}};
static imonitorplugin::PluginData data("unit_test_plugin", 1546329600,
                                       data_vec);

TEST_CASE("Sqlite3 adapter test", "[PersistenceService]") {
  auto settings = std::make_unique<persistenceservice::sqlite::SqliteSettings>(
      database_file);
  auto adapter_factory =
      new persistenceservice::AdapterFactory(std::move(settings));

  // A section is used to ensure the database file is removed, even if a test
  // statement fails
  SECTION("Adapter test") {
    std::unique_ptr<persistenceservice::IDataAdapter> adapter;
    REQUIRE_NOTHROW([&]() { adapter = adapter_factory->CreateAdapter(); }());
    REQUIRE(access(database_file.c_str(), F_OK) != -1);

    REQUIRE_NOTHROW([&]() { adapter->Store(data); }());

    std::vector<imonitorplugin::PluginData> records;
    REQUIRE_NOTHROW([&]() { records = adapter->Load(data.plugin_name()); }());
    REQUIRE(records.size() == 1);
    REQUIRE(records.front().ToString() == data.ToString());

    utility::datastructure::Table table;
    table = adapter->LoadTable(data.plugin_name());
    REQUIRE(table.name() == data.plugin_name());
    std::vector<std::string> col_names{"c_int",     "c_float",  "c_double",
                                       "c_int64_t", "c_string", "timestamp"};
    REQUIRE_THAT(table.GetColumnNames(),
                 Catch::Matchers::UnorderedEquals(col_names));
    // sqlite3 does not differentiate float and double.
    REQUIRE(table.GetDataColumn<double>("c_float").data().front() ==
            Approx(17.17f));
  }
  std::remove(database_file.c_str());
}
