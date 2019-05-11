#include "reporter/templateprocessor/templatetoken.hpp"

reporter::templateprocessor::TemplateToken::TemplateToken(
    std::string key, std::function<std::string()> value_func, bool cache)
    : key_(key), value_func_(value_func), cache_(cache) {}

std::string reporter::templateprocessor::TemplateToken::key() const {
  return key_;
}

std::string reporter::templateprocessor::TemplateToken::value() {
  if (this->cache_ && !this->value_.empty()) {
    return this->value_;
  }
  return this->value_ = this->value_func_();
}
