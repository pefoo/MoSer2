#include "settingsprovider/settingsprovider.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "catch2/catch.hpp"
#include "settingsprovider/settingidentifier.hpp"
#include "settingsprovider/settingsfactory.hpp"
#include "settingsprovider/settingverifier.hpp"

std::string GetTestFile(const std::string& file_name) {
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

  std::vector<std::string> exptected_sections = {"s1", "s2"};
  REQUIRE_THAT(exptected_sections,
               Catch::UnorderedEquals(settings->Sections()));
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

TEST_CASE("Settings provider write to file", "[Settingsprovider]") {
  SettingIdentifier s1{"foobar", "s1"};
  SettingIdentifier s2{"f", "s1"};
  SettingIdentifier s3{"hi", "s2"};
  SettingIdentifier s4{"foo", ""};
  SettingIdentifier s5{"fo2", ""};

  SettingsFactory factory{};
  factory.RegisterSetting(s1, "s1");
  factory.RegisterSetting(s2, "s2");
  factory.RegisterSetting(s3, "0", TypeVerifier<int>::VerifierFunc);
  factory.RegisterSetting(s4, "s4");
  factory.RegisterSetting(s5, "s5");

  factory.WriteToFile("dummy.conf");

  auto ref_file = GetTestFile("config_write.conf");
  std::ifstream ref_stream{ref_file};
  std::string ref{(std::istreambuf_iterator<char>(ref_stream)),
                  std::istreambuf_iterator<char>()};
  std::ifstream actual_stream{"dummy.conf"};
  std::string actual{(std::istreambuf_iterator<char>(actual_stream)),
                     std::istreambuf_iterator<char>()};

  REQUIRE(ref == actual);
}
