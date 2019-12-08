#ifndef PROCESSINFORMATIONVIEWS_HPP
#define PROCESSINFORMATIONVIEWS_HPP

#include "reporter/viewmodel/processinformationviewmodel.hpp"
#include <memory>

namespace reporter {
namespace views {

///
/// \brief Provides different process information related widgets
///
class ProcessInformationViews
{
public:
  ///
  /// \brief Create a new instance
  /// \param model The view model to use
  ///
  explicit ProcessInformationViews(std::shared_ptr<reporter::viewmodel::ProcessInformationViewModel> model);

  ///
  /// \brief Get a html table with the top memory using processes
  /// \param count The number of rows
  /// \return The html table as string
  ///
  std::string GetTopMemoryUsingProcessesWidget(int count);

  ///
  /// \brief Get a html table with the top disk write causing processes
  /// \param count The number of rows
  /// \return The html table as string
  ///
  std::string GetTopDiskWriteProcessWidget(int count);

  ///
  /// \brief Get a html table with the top disk read causing processes
  /// \param count The number of rows
  /// \return The html table as string
  ///
  std::string GetTopDiskReadProcessWidget(int count);

private:
  std::shared_ptr<reporter::viewmodel::ProcessInformationViewModel> model_;
};

} // namespace views
} // namespace reporter


#endif // PROCESSINFORMATIONVIEWS_HPP
