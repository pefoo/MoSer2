#ifndef DATALESSTOKENFACTORY_H
#define DATALESSTOKENFACTORY_H

#include <vector>
#include <string>
#include "reporter/templateprocessor/templatetoken.hpp"

namespace reporter {
namespace tokens {

///
/// \brief Create a token
/// \details Creates a token with default signature
/// \param token_name The name of the token
/// \param body The body of the lambda expression for the token
///
#define CREATE_TOKEN(token_name, body)           \
  ::reporter::templateprocessor::TemplateToken { \
    token_name, []() -> std::string { body }     \
  }

///
/// \brief Create a token
/// \details Creates a token with default signature
/// \param token_name The name of the token
/// \param capture A capture expression for the lambda
/// \param body The body of the lambda expression for the token
///
#define CREATE_TOKEN_CAPTURE(token_name, capture, body) \
  ::reporter::templateprocessor::TemplateToken {        \
    token_name, [capture]() -> std::string { body }     \
  }

///
/// \brief Create tokens that do not require any data input
/// \return A vector with template tokens
///
std::vector<templateprocessor::TemplateToken> GetDatalessTokens();

}  // namespace tokens
}  // namespace reporter

#endif  // DATALESSTOKENFACTORY_H
