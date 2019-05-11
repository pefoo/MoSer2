#include "reporter/templateprocessor/templateprocessor.hpp"
#include <filesystem>
#include <fstream>
#include "utility/filesystem/fileaccesshelper.hpp"

reporter::templateprocessor::TemplateProcessor::TemplateProcessor(
    std::vector<reporter::templateprocessor::TemplateToken> tokens)
    : tokens_(tokens) {}

std::string reporter::templateprocessor::TemplateProcessor::ProcessTemplate(
    std::string template_file, bool in_place) {
  auto result_file = in_place
                         ? template_file
                         : utility::filesystem::PathCombine(
                               {std::filesystem::temp_directory_path().string(),
                                std::to_string(std::time(nullptr))})
                               .string();

  std::stringstream buffer;
  std::ifstream template_stream(template_file);
  std::string line;

  while (std::getline(template_stream, line)) {
    std::string processed_line = line;
    for (auto& token : this->tokens_) {
      processed_line = this->ReplaceAll(processed_line, token);
    }
    buffer << processed_line;
  }
  template_stream.close();

  std::ofstream out(result_file, std::ios::trunc);
  out << buffer.rdbuf();

  return result_file;
}

std::string reporter::templateprocessor::TemplateProcessor::ReplaceAll(
    std::string subject, TemplateToken& token) {
  size_t pos = 0;
  while ((pos = subject.find(token.key(), pos)) != std::string::npos) {
    subject.replace(pos, token.key().length(), token.value());
    pos += token.value().size();
  }
  return subject;
}
