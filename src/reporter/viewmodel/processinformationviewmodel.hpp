#ifndef PROCESSINFORMATIONVIEWMODEL_HPP
#define PROCESSINFORMATIONVIEWMODEL_HPP

#include <vector>
#include "reporter/helper/processinformationhelper.hpp"
#include "reporter/datamodel/processinformation.hpp"

namespace reporter {
namespace viewmodel {

///
/// \brief View model for process information widgets
/// \details Bind it to a view to provide data access
///
class ProcessInformationViewModel
{
public:
  ///
  /// \brief Create a new instance
  /// \note This will populate the process information.
  ///
  ProcessInformationViewModel();

  ///
  /// \brief Refresh process information
  ///
  void Refresh();

  ///
  /// \brief Get the top x memory using processes
  /// \param count The number of processes to return
  /// \return The top memory using processes
  ///
  std::vector<reporter::datamodel::ProcessInformation> GetTopMemoryUsingProcesses(int count);

  ///
  /// \brief Get the top x disk write causing processes
  /// \param count The number of processes to return
  /// \return The top disk write causing processes
  ///
  std::vector<reporter::datamodel::ProcessInformation> GetTopDiskWriteProcesses(int count);

  ///
  /// \brief Get the top x disk read causing processes
  /// \param count The number of processes to return
  /// \return The top disk read causing processes
  ///
  std::vector<reporter::datamodel::ProcessInformation> GetTopDiskReadProcesses(int count);

private:
  std::vector<reporter::datamodel::ProcessInformation> process_information_;
};


} // namespace viewmodel

} // namespace reporter
#endif // PROCESSINFORMATIONVIEWMODEL_HPP
