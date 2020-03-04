#include "reporter/templateprocessor/templateprocessor.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include "constants/settings/settingsidentifier.hpp"
#include "easyloggingpp-9.96.5/easylogging++.h"
#include "settingsprovider/settingnotfoundexception.hpp"
#include "utility/filesystem/fileaccesshelper.hpp"

reporter::templateprocessor::TemplateProcessor::TemplateProcessor(
    std::vector<reporter::templateprocessor::TemplateToken> tokens,
    std::shared_ptr<settingsprovider::ISettingsProvider> settings)
    : tokens_(std::move(tokens)), settings_(std::move(settings)) {}

std::string reporter::templateprocessor::TemplateProcessor::ProcessTemplate(
    const std::string& template_file, bool in_place) {
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
      processed_line = this->ReplaceAll(processed_line, &token);
    }
    buffer << processed_line << std::endl;
  }
  template_stream.close();

  std::ofstream out(result_file, std::ios::trunc);
  out << buffer.rdbuf();

  try {
    if (this->settings_) {
      auto script_path = this->settings_->GetValue(
          constants::settings::PostProcessingScript());
      if (!std::filesystem::exists(script_path)) {
        throw std::runtime_error("The script " + script_path +
                                 " does not exist.");
      }
      std::string cmd = script_path + " '" + result_file + "'";
      if (auto ret = std::system(cmd.c_str()) != 0) {
        throw std::runtime_error("Execution of script " + script_path +
                                 " failed with return code " +
                                 std::to_string(ret));
      }
    }
  } catch (settingsprovider::SettingNotFoundException&) {
    // We dont care for not configured postprocessing scripts
  } catch (std::exception& e) {
    LOG(ERROR) << "Failed to execute the report postprocessing script. "
               << e.what();
  }

  return result_file;
}

std::string reporter::templateprocessor::TemplateProcessor::ReplaceAll(
    std::string subject, TemplateToken* token) {
  size_t pos = 0;
  while ((pos = subject.find(token->key(), pos)) != std::string::npos) {
    subject.replace(pos, token->key().length(), token->value());
    pos += token->value().size();
  }
  return subject;
}
