#include "settingsprovider/settingsprovider.hpp"
#include <sstream>
#include <string>
#include "catch2/catch.hpp"

std::string GetTestFile(const std::string file_name) {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << file_name;
  return ss.str();
}

TEST_CASE("Settings provider registered settings", "[Settingsprovider]") {
  settingsprovider::SettingsProvider sp{};
  auto s1 = new settingsprovider::Setting<std::string>{"foobar", "s1"};
  sp.RegisterSetting(s1);
  sp.RegisterSetting(new settingsprovider::Setting<std::string>{"f", "s1"});
  sp.RegisterSetting(new settingsprovider::Setting<int>{"hi", "s2"});

  std::vector<std::string> errors;
  REQUIRE(sp.ReadFromFile(GetTestFile("valid_settings.conf"), &errors));
  REQUIRE(errors.size() == 0);

  REQUIRE(sp.GetValue<std::string>(s1->key(), s1->section()) == "bar");
  REQUIRE(sp.GetValue<std::string>("f", "s1") == "b");
  REQUIRE(sp.GetValue<int>("hi", "s2") == 10);
}

TEST_CASE("Settings provider registered settings wrong type",
          "[Settingsprovider]") {
  settingsprovider::SettingsProvider sp{};
  sp.RegisterSetting(new settingsprovider::Setting<int>{"foobar", "s1"});

  std::vector<std::string> errors;
  REQUIRE_FALSE(sp.ReadFromFile(GetTestFile("valid_settings.conf"), &errors));
  REQUIRE(errors.size() == 1);
}

TEST_CASE("Settings provider registered settings wrong setting",
          "[Settingsprovider]") {
  settingsprovider::SettingsProvider sp{};
  sp.RegisterSetting(new settingsprovider::Setting<int>{
      "hi", "s2", 0, [](const int& v, std::string* msg) {
        *msg = "I dont want that value! " + std::to_string(v);
        return false;
      }});

  std::vector<std::string> errors;
  REQUIRE_FALSE(sp.ReadFromFile(GetTestFile("valid_settings.conf"), &errors));
  REQUIRE(errors.size() == 1);
}

TEST_CASE("Settings provider unregistered settings", "[Settingsprovider]") {
  settingsprovider::SettingsProvider sp{};
  std::vector<std::string> errors;
  REQUIRE(sp.ReadFromFile(GetTestFile("valid_settings.conf"), &errors));
  REQUIRE(errors.size() == 0);

  REQUIRE(sp.GetValue<std::string>("foobar", "s1") == "bar");
  REQUIRE(sp.GetValue<std::string>("f", "s1") == "b");
  REQUIRE_THROWS(sp.GetValue<int>("hi", "s2"));
  REQUIRE(sp.GetValue<std::string>("hi", "s2") == "10");
}

TEST_CASE("Settings provider invalid file", "[Settingsprovider]") {
  settingsprovider::SettingsProvider sp{};
  std::vector<std::string> errors;
  REQUIRE_FALSE(sp.ReadFromFile(GetTestFile("not existing file"), &errors));
  REQUIRE(errors.size() == 1);

  errors.clear();
  REQUIRE_FALSE(sp.ReadFromFile(GetTestFile("invalid_settings.conf"), &errors));
}
