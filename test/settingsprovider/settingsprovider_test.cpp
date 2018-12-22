#include "settingsprovider/settingsprovider.hpp"
#include <sstream>
#include <string>
#include "catch2/catch.hpp"
#include "settingsprovider/settingidentifier.hpp"
#include "settingsprovider/settingsfactory.hpp"
#include "settingsprovider/settingverifier.hpp"

std::string GetTestFile(const std::string file_name) {
  std::stringstream ss{};
  ss << TESTDATA_DIR;
  ss << file_name;
  return ss.str();
}

using namespace settingsprovider;

TEST_CASE("Settings provider with registered settings", "[SettingsProvider]") {
  SettingIdentifier s1{"foobar", "s1"};
  SettingIdentifier s2{"f", "s1"};
  SettingIdentifier s3{"hi", "s2"};

  SettingsFactory factory{};
  factory.RegisterSetting(s1);
  factory.RegisterSetting(s2);
  factory.RegisterSetting(s3, "0", TypeVerifier<int>::VerifierFunc);
  std::vector<std::string> errors;
  auto settings =
      factory.ReadFromFile(GetTestFile("valid_settings.conf"), &errors);
  REQUIRE(errors.size() == 0);
  REQUIRE(settings->GetValue(s1) == "bar");
  REQUIRE(settings->GetValue(s2) == "b");
  REQUIRE(settings->GetValue(s3) == "10");
}

TEST_CASE("Settings provider registered settings wrong type",
          "[Settingsprovider]") {
  SettingIdentifier s1{"foobar", "s1"};

  SettingsFactory factory{};
  factory.RegisterSetting(s1, "0", TypeVerifier<double>::VerifierFunc);
  std::vector<std::string> errors;
  auto settings =
      factory.ReadFromFile(GetTestFile("valid_settings.conf"), &errors);
  REQUIRE(errors.size() == 1);
}

TEST_CASE("Settings provider unregistered settings", "[Settingsprovider]") {
  SettingIdentifier s1{"foobar", "s1"};
  SettingIdentifier s2{"f", "s1"};
  SettingIdentifier s3{"hi", "s2"};

  SettingsFactory factory{};
  std::vector<std::string> errors;
  auto settings =
      factory.ReadFromFile(GetTestFile("valid_settings.conf"), &errors);
  REQUIRE(errors.size() == 0);
  REQUIRE(settings->GetValue(s1) == "bar");
  REQUIRE(settings->GetValue(s2) == "b");
  REQUIRE(settings->GetValue(s3) == "10");
}
