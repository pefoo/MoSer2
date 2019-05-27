#ifndef EMAILSENDER_H
#define EMAILSENDER_H

#include <string>
#include <vector>

namespace reporter {
namespace email {

///
/// \brief Send the report via email
/// \details Tested with mail gun
/// \param report_file The report file to send
/// \param recipients The mail recipients
/// \param smtp_server The smtp server to use
/// \param user The user to login
/// \param password The password of the user
/// \return True, if the email was send
///
bool SendReport(const std::string& report_file,
                std::vector<std::string> recipients,
                const std::string& smtp_server, const std::string& user,
                const std::string password);

///
/// \brief Send the report via email
/// \details Tested with mail gun
/// \param report_file The report file to send
/// \param recipient The mail recipient
/// \param smtp_server The smtp server to use
/// \param user The user to login
/// \param password The password of the user
/// \return True, if the email was send
///
bool SendReport(const std::string& report_file, const std::string& recipient,
                const std::string& smtp_server, const std::string& user,
                const std::string password);

}  // namespace email
}  // namespace reporter

#endif  // EMAILSENDER_H
