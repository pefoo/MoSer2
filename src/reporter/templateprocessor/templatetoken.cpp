#include "reporter/templateprocessor/templatetoken.hpp"
#include <string>
#include "easyloggingpp-9.96.5/src/easylogging++.h"

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
  try {
    return this->value_ = this->value_func_();
  } catch (std::exception &e) {
    LOG(ERROR) << "Failed to get value of token " << this->key()
               << " Error: " << e.what();
    return "";
  }
}
