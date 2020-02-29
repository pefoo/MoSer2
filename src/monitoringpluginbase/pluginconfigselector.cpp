#include "monitoringpluginbase/pluginconfigselector.hpp"
#include <utility>

monitoringpluginbase::PluginConfigSelector::PluginConfigSelector(
    std::string key,
    const monitoringpluginbase::PluginConfigSelector::Selector &selector)
    : key_(std::move(key)), selector_(selector) {}

monitoringpluginbase::PluginConfigSelector::PluginConfigSelector(
    std::string key, std::vector<std::string> applicable_values,
    std::string help_text)
    : key_(std::move(key)) {
  this->selector_ = this->BuildSelector(help_text, applicable_values);
}

monitoringpluginbase::PluginConfigSelector::PluginConfigSelector(
    std::string key, const std::string &value)
    : key_(std::move(key)), selector_([value]() { return value; }) {}

std::tuple<std::string, std::string>
monitoringpluginbase::PluginConfigSelector::SelectConfig() const {
  return {this->key_, this->selector_()};
}

std::function<std::string()>
monitoringpluginbase::PluginConfigSelector::BuildSelector(
    const std::string &help_text, std::vector<std::string> applicable_values) {
  {
    return [help_text, applicable_values]() {
      std::cout << help_text << std::endl;
      std::string value;
      bool is_acceptable_input{false};
      do {
        std::cout << "Possible values: " << std::endl;
        for (const auto &v : applicable_values) {
          std::cout << v << std::endl;
        }
        std::cout << ">>";

        getline(std::cin, value);
        is_acceptable_input =
            std::find(applicable_values.begin(), applicable_values.end(),
                      value) != applicable_values.end();
        if (!is_acceptable_input) {
          std::cout << "The value you entered is incorrect!" << std::endl;
        }
      } while (!is_acceptable_input);
      return value;
    };
  }
}
