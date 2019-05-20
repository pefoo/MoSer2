#ifndef TEMPLATEPROCESSOR_H
#define TEMPLATEPROCESSOR_H

#include <string>
#include <vector>
#include "reporter/templateprocessor/templatetoken.hpp"

namespace reporter {
namespace templateprocessor {

///
/// \brief The template processor
///
class TemplateProcessor {
 public:
  ///
  /// \brief Create a new instance
  /// \param tokens The tokens to be used for replacement
  ///
  explicit TemplateProcessor(std::vector<TemplateToken> tokens);

  ///
  /// \brief Process a template file
  /// \param template_file The file to process
  /// \param in_place A flag, indicating whether the result is written back to
  /// the source file
  /// \return The path to the result file
  /// \details If in_place is set to false, a new file is generated. The new
  /// file will be located in some temp path
  ///
  std::string ProcessTemplate(std::string template_file, bool in_place = false);

 private:
  std::vector<TemplateToken> tokens_;

  std::string ReplaceAll(std::string subject, TemplateToken* token);
};
}  // namespace templateprocessor
}  // namespace reporter

#endif  // TEMPLATEPROCESSOR_H
