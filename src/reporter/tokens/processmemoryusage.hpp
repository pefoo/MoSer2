#ifndef PROCESSMEMORYUSAGE_HPP
#define PROCESSMEMORYUSAGE_HPP

#include <string>
#include <vector>

namespace reporter {
namespace tokens {

///
/// \brief Captures memory usage by process at creation time.
/// \note Any Token value created by this class is using the data captured
/// at creation time.
///
class ProcessMemoryUsage {
 public:
  ///
  /// \brief Create a new instance
  ///
  ProcessMemoryUsage();

  ///
  /// \brief ~ProcessMemoryUsage
  ///
  ~ProcessMemoryUsage();

  ///
  /// \brief Get the memory usage by process table
  /// \param entries The number of entries
  /// \return A table for the report
  ///
  std::string GetMemoryUsageByProcessTable(uint entries);

 private:
  struct ProcessStatus;
  std::vector<ProcessStatus> proc_status_;
  void ReadProcessStatus();
};
}  // namespace tokens
}  // namespace reporter

#endif  // PROCESSMEMORYUSAGE_HPP
