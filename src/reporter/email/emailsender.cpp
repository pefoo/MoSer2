#include "emailsender.hpp"
#include <curl/curl.h>
#include <filesystem>
#include "easyloggingpp-9.96.5/src/easylogging++.h"

class CurlRaiiWrapper {
 public:
  CurlRaiiWrapper() : curl_(curl_easy_init()) {}
  ~CurlRaiiWrapper() { curl_easy_cleanup(this->curl_); }
  CURL *Instance() { return this->curl_; }

 private:
  CURL *curl_;
};

class CurlRecipientsRaiiWrapper {
 public:
  CurlRecipientsRaiiWrapper() : recipients_(nullptr) {}
  ~CurlRecipientsRaiiWrapper() { curl_slist_free_all(this->recipients_); }
  void AddRecipient(const std::string recipient) {
    this->recipients_ = curl_slist_append(this->recipients_, recipient.c_str());
  }
  curl_slist *Instante() { return this->recipients_; }

 private:
  curl_slist *recipients_;
};

bool reporter::email::SendReport(const std::string &report_file,
                                 const std::string &recipient,
                                 const std::string &smtp_server,
                                 const std::string &user,
                                 const std::string password) {
  return SendReport(report_file, std::vector<std::string>{recipient},
                    smtp_server, user, password);
}

bool reporter::email::SendReport(const std::string &report_file,
                                 std::vector<std::string> recipients,
                                 const std::string &smtp_server,
                                 const std::string &user,
                                 const std::string password) {
  if (!std::filesystem::exists(report_file)) {
    LOG(ERROR) << "The report file " << report_file << " does not exist.";
    return false;
  }
  CurlRaiiWrapper curl;
  CurlRecipientsRaiiWrapper curl_recipients;
  if (!curl.Instance()) {
    LOG(ERROR) << "Failed to initialize curl.";
    return false;
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdisabled-macro-expansion"
  // basic server setup
  curl_easy_setopt(curl.Instance(), CURLOPT_URL, smtp_server.c_str());
  curl_easy_setopt(curl.Instance(), CURLOPT_USERNAME, user.c_str());
  curl_easy_setopt(curl.Instance(), CURLOPT_PASSWORD, password.c_str());
  // sender, recipient
  curl_easy_setopt(curl.Instance(), CURLOPT_MAIL_FROM, user.c_str());
  for (const auto &rec : recipients) {
    curl_recipients.AddRecipient(rec);
  }
  curl_easy_setopt(curl.Instance(), CURLOPT_MAIL_RCPT,
                   curl_recipients.Instante());

  // mail content
  FILE *mail_content = fopen(report_file.c_str(), "r");
  curl_easy_setopt(curl.Instance(), CURLOPT_READDATA, mail_content);
  curl_easy_setopt(curl.Instance(), CURLOPT_UPLOAD, 1L);

#pragma GCC diagnostic pop

  auto result = curl_easy_perform(curl.Instance());
  if (result != CURLE_OK) {
    std::string error = curl_easy_strerror(result);
    LOG(ERROR) << "Failed to send the mail: " << error;
    return false;
  }
  return true;
}
