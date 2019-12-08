#ifndef PROCESSINFORMATIONHELPER_HPP
#define PROCESSINFORMATIONHELPER_HPP

#include <vector>
#include "reporter/datamodel/processinformation.hpp"

namespace reporter {
namespace helper {

///
/// \brief Get information about all running processes
/// \return A list of process information
///
std::vector<reporter::datamodel::ProcessInformation> GetProcessInformation();

} // namespace helper
} // namespace reporter


#endif // PROCESSINFORMATIONHELPER_HPP
