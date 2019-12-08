#include "reporter/viewmodel/processinformationviewmodel.hpp"
#include <algorithm>

reporter::viewmodel::ProcessInformationViewModel::
    ProcessInformationViewModel() {
  this->process_information_ = reporter::helper::GetProcessInformation();
}

void reporter::viewmodel::ProcessInformationViewModel::Refresh() {
  for (auto& pi : this->process_information_) {
    pi.Refresh();
  }
}

std::vector<reporter::datamodel::ProcessInformation>
reporter::viewmodel::ProcessInformationViewModel::GetTopMemoryUsingProcesses(
    int count) {
  std::sort(this->process_information_.begin(),
            this->process_information_.end(),
            [](const reporter::datamodel::ProcessInformation& a,
               const reporter::datamodel::ProcessInformation& b) {
              return a.anonymous_memory() > b.anonymous_memory();
            });

  return std::vector<reporter::datamodel::ProcessInformation>{
      this->process_information_.begin(),
      this->process_information_.begin() + count};
}

std::vector<reporter::datamodel::ProcessInformation>
reporter::viewmodel::ProcessInformationViewModel::GetTopDiskWriteProcesses(
    int count) {
  std::sort(this->process_information_.begin(),
            this->process_information_.end(),
            [](const reporter::datamodel::ProcessInformation& a,
               const reporter::datamodel::ProcessInformation& b) {
              return a.disk_write() > b.disk_write();
            });

  return std::vector<reporter::datamodel::ProcessInformation>{
      this->process_information_.begin(),
      this->process_information_.begin() + count};
}

std::vector<reporter::datamodel::ProcessInformation>
reporter::viewmodel::ProcessInformationViewModel::GetTopDiskReadProcesses(
    int count) {
  std::sort(this->process_information_.begin(),
            this->process_information_.end(),
            [](const reporter::datamodel::ProcessInformation& a,
               const reporter::datamodel::ProcessInformation& b) {
              return a.disk_read() > b.disk_read();
            });

  return std::vector<reporter::datamodel::ProcessInformation>{
      this->process_information_.begin(),
      this->process_information_.begin() + count};
}
