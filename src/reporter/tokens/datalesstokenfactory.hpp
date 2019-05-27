#ifndef DATALESSTOKENFACTORY_H
#define DATALESSTOKENFACTORY_H

#include <vector>
#include "reporter/templateprocessor/templatetoken.hpp"

namespace reporter {
namespace tokens {

///
/// \brief Create tokens that do not require any data input
/// \return A vector with template tokens
///
std::vector<templateprocessor::TemplateToken> GetDatalessTokens();

}  // namespace tokens
}  // namespace reporter

#endif  // DATALESSTOKENFACTORY_H
