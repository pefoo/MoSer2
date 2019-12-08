#include "reporter/views/processinformationviews.hpp"
#include <cmath>
#include <sstream>
#include <utility>
#include "reporter/views/HtmlTokens.hpp"

static constexpr char kCustomTableColumnStart[] =
    "<td class=default style='padding:4px'>";

reporter::views::ProcessInformationViews::ProcessInformationViews(
    std::shared_ptr<viewmodel::ProcessInformationViewModel> model)
    : model_(std::move(model)) {}

std::string
reporter::views::ProcessInformationViews::GetTopMemoryUsingProcessesWidget(
    int count) {
  std::stringstream ss;
  ss << defaultclass::kTableStart << defaultclass::kTableRowStart;
  ss << defaultclass::kTableHeaderStart << "Process" << kTableHeaderEnd;
  ss << defaultclass::kTableHeaderStart << "Memory [MB]" << kTableHeaderEnd;
  ss << defaultclass::kTableHeaderStart << "Command line" << kTableHeaderEnd;
  ss << kTableRowEnd;

  for (const auto& info : this->model_->GetTopMemoryUsingProcesses(count)) {
    ss << defaultclass::kTableRowStart;
    ss << kCustomTableColumnStart << info.name() << kTableColumnEnd;
    ss << kCustomTableColumnStart
       << std::round(pow(2.0, -10.0) * info.anonymous_memory())
       << kTableColumnEnd;
    ss << kCustomTableColumnStart << info.cmd_line() << kTableColumnEnd;
    ss << kTableRowEnd;
  }
  ss << kTableEnd;
  return ss.str();
}

std::string
reporter::views::ProcessInformationViews::GetTopDiskWriteProcessWidget(
    int count) {
  std::stringstream ss;
  ss << defaultclass::kTableStart << defaultclass::kTableRowStart;
  ss << defaultclass::kTableHeaderStart << "Process" << kTableHeaderEnd;
  ss << defaultclass::kTableHeaderStart << "Disk write [MB]" << kTableHeaderEnd;
  ss << defaultclass::kTableHeaderStart << "Command line" << kTableHeaderEnd;
  ss << kTableRowEnd;

  for (const auto& info : this->model_->GetTopDiskWriteProcesses(count)) {
    ss << defaultclass::kTableRowStart;
    ss << kCustomTableColumnStart << info.name() << kTableColumnEnd;
    ss << kCustomTableColumnStart << std::round(info.disk_write() / 10000000)
       << kTableColumnEnd;
    ss << kCustomTableColumnStart << info.cmd_line() << kTableColumnEnd;
    ss << kTableRowEnd;
  }
  ss << kTableEnd;
  return ss.str();
}

std::string
reporter::views::ProcessInformationViews::GetTopDiskReadProcessWidget(
    int count) {
  std::stringstream ss;
  ss << defaultclass::kTableStart << defaultclass::kTableRowStart;
  ss << defaultclass::kTableHeaderStart << "Process" << kTableHeaderEnd;
  ss << defaultclass::kTableHeaderStart << "Disk read [MB]" << kTableHeaderEnd;
  ss << defaultclass::kTableHeaderStart << "Command line" << kTableHeaderEnd;
  ss << kTableRowEnd;

  for (const auto& info : this->model_->GetTopDiskReadProcesses(count)) {
    ss << defaultclass::kTableRowStart;
    ss << kCustomTableColumnStart << info.name() << kTableColumnEnd;
    ss << kCustomTableColumnStart << std::round(info.disk_read() / 10000000);
    ss << kCustomTableColumnStart << info.cmd_line() << kTableColumnEnd;
    ss << kTableRowEnd;
  }
  ss << kTableEnd;
  return ss.str();
}
