#include "pluginloader.hpp"
#include <vector>
#include "catch2/catch.hpp"
#include "pluginmanager.hpp"
#include "sample_lib/HelloBase.hpp"

TEST_CASE("Plugin Loader") {
  auto pl = new pluginmanager::PluginLoader<
      HelloBase, pluginmanager::FunctionTypes<
                     HelloBase>::ParameterizedCreateType<const std::string&>>{
      "./libsample_lib.so"};

  auto instance = pl->CreateInstance("Hi");
  REQUIRE(instance->Instance()->Hi() == "Hi");

  instance->Instance()->Inc();
  REQUIRE(instance->Instance()->GetI() == 1);

  auto i2 = pl->CreateInstance("Bye");
  REQUIRE(i2->Instance()->Hi() == "Bye");

  REQUIRE(instance->Instance()->Hi() == "Hi");

  instance.reset();
  i2.reset();

  delete pl;
}
