#include <unistd.h>
#include <cstdio>
#include <string>
#include "catch2/catch.hpp"
#include "persistenceservice/adapterfactory.hpp"
#include "persistenceservice/adaptersettings.hpp"
#include "persistenceservice/idataadapter.hpp"
#include "persistenceservice/sqlite/sqliteadapter.hpp"
#include "persistenceservice/sqlite/sqlitesettings.hpp"

const std::string database_file = "sqlite3_adapter_test.db";
static imonitorplugin::PluginData::data_vector data_vec = {
    {"c_int", utility::datastructure::Any(17)},
    {"c_float", utility::datastructure::Any(17.17f)},
    {"c_double", utility::datastructure::Any(17.17)},
    {"c_int64_t", utility::datastructure::Any(int64_t(17))},
    {"c_string", utility::datastructure::Any(std::string("foobar"))}};
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
  }
  std::remove(database_file.c_str());
}
