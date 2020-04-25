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
      /* Double check existence. From directory_iterator docs:
       * "If a file or a directory is deleted or added to the directory tree
       * after the directory iterator has been created, it is unspecified
       * whether the change would be observed through the iterator."
       *
       * Thus, the iterator might (and actually did in the past) return ids of
       * processes that do not exist anymore.
       */
      if (!std::filesystem::exists(entry)) continue;
      procs.push_back(
          reporter::datamodel::ProcessInformation{std::stoul(sm[1])});
    }
  }
  std::sort(procs.begin(), procs.end());
  procs.erase(std::unique(procs.begin(), procs.end(),
                          [](const reporter::datamodel::ProcessInformation& a,
                             const reporter::datamodel::ProcessInformation& b) {
                            return (a.name() == b.name()) &&
                                   (a.cmd_line() == b.cmd_line());
                          }),
              procs.end());
  return procs;
}
