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

TEST_CASE("Sqlite3 adapter test", "[PersistenceService]") {
  // use sections to ensure the setup and teardown are executed

  // setup
  auto settings = std::make_unique<persistenceservice::sqlite::SqliteSettings>(
      database_file);
  auto adapter_factory =
      new persistenceservice::AdapterFactory(std::move(settings));

  SECTION("Open database") {
    std::unique_ptr<persistenceservice::IDataAdapter> adapter;
    REQUIRE_NOTHROW([&]() { adapter = adapter_factory->CreateAdapter(); }());
    REQUIRE(access(database_file.c_str(), F_OK) != -1);
  }

  // teardown
  // TODO might want to rename the test database files for debugging purpose
  std::remove(database_file.c_str());
}
