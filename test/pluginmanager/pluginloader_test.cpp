#include "pluginloader.hpp"
#include <sstream>
#include <string>
#include <vector>
#include "catch2/catch.hpp"
#include "pluginmanager.hpp"
#include "sample_lib/HelloBase.hpp"

std::string GetLibrary() {
  std::stringstream ss{};
  ss << LIBRARY_DIR << "/";
  ss << "libsample_lib.so";
  return ss.str();
}

TEST_CASE("Plugin Loader", "[PluginManager]") {
  auto pl = new pluginmanager::PluginLoader<
      HelloBase, pluginmanager::FunctionTypes<
                     HelloBase>::ParameterizedCreateType<const std::string&>>{
      GetLibrary()};

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
