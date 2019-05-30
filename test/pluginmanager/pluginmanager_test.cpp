#include "pluginmanager/pluginmanager.hpp"
#include "catch2/catch.hpp"
#include "sample_lib/HelloBase.hpp"

TEST_CASE("Plugin manager") {
  auto manager = new pluginmanager::PluginManager<
      HelloBase,
      pluginmanager::FunctionTypes<HelloBase>::ParameterizedCreateType<
          const std::string&>,
      pluginmanager::FunctionTypes<HelloBase>::DefaultDestroyType>{};

  auto plugin =
      manager->LoadPlugin("./libsample_lib.so", "create", "destroy", "foobar");
  auto plugin2 =
      manager->LoadPlugin("./libsample_lib.so", "create", "destroy", "2");

  REQUIRE(plugin->Instance()->Hi() == "foobar");
  REQUIRE(plugin2->Instance()->Hi() == "2");

  manager->DestroyPlugin(plugin2);
  REQUIRE(plugin->Instance()->Hi() == "foobar");
  manager->DestroyPlugin(plugin);

  REQUIRE(plugin == nullptr);
  REQUIRE(plugin2 == nullptr);

  plugin =
      manager->LoadPlugin("./libsample_lib.so", "create", "destroy", "foobar");
  plugin2 = manager->LoadPlugin("./libsample_lib.so", "create", "destroy", "2");

  manager->DestroyAll();

  REQUIRE_THROWS_AS(plugin->Instance()->Hi(), std::runtime_error);

  delete manager;
}
