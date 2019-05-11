#include "reporter/templateprocessor/templatetoken.hpp"

TemplateToken::TemplateToken(std::string key,
                             std::function<std::string()> value_func,
                             bool cache)
    : key_(key), value_func_(value_func), cache_(cache) {}

std::string TemplateToken::key() const { return key_; }

std::string TemplateToken::value() {
  if (this->cache_ && !this->value_.empty()) {
    return this->value_;
  }
  return this->value_ = this->value_func_();
}
