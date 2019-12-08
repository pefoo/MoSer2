#include "reporter/helper/processinformationhelper.hpp"
#include <filesystem>
#include <utility/helper/stringhelper.hpp>

using namespace utility::helper;

std::vector<reporter::datamodel::ProcessInformation>
reporter::helper::GetProcessInformation() {
  std::vector<reporter::datamodel::ProcessInformation> procs;
  std::smatch sm;
  for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
    if (entry.is_directory() &&
        StringRgxGrep(entry.path(), R"(^/proc/(\d+)$)", &sm)) {
      procs.push_back(
          reporter::datamodel::ProcessInformation{std::stoul(sm[1])});
    }
  }
  return procs;
}
