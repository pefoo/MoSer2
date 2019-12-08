#ifndef PROCESSINFORMATION_HPP
#define PROCESSINFORMATION_HPP

#include <string>

namespace reporter {
namespace datamodel {

///
/// \brief Process related information.
/// \details Information is taken from /proc/{pid}/{...}
/// \note Parsing is done once during construction.
///
class ProcessInformation {
 public:
  ///
  /// \brief Create a new instance for a process
  /// \param pid The id of the process
  ///
  explicit ProcessInformation(size_t pid);

  ///
  /// \brief Refresh the stats
  ///
  void Refresh();

  ///
  /// \brief Get name
  /// \return The value of name
  ///
  [[nodiscard]] std::string name() const;

  ///
  /// \brief Get cmd_line
  /// \return The value of cmd_line
  ///
  [[nodiscard]] std::string cmd_line() const;

  ///
  /// \brief Get anonymous_memory in KB
  /// \return The value of anonymous_memory
  ///
  [[nodiscard]] size_t anonymous_memory() const;

  ///
  /// \brief Get virtual_memory in KB
  /// \return The value of virtual_memory
  ///
  [[nodiscard]] size_t virtual_memory() const;

  ///
  /// \brief Get pid
  /// \return The value of pid
  ///
  [[nodiscard]] size_t pid() const;

  ///
  /// \brief Get bytes read by this process
  /// \return Number of bytes this process caused to fetch form storage layer
  /// \note This value is accurate for block backed filesystems only. This value
  /// is only available for processes started by the current user. Run Moserep
  /// as root to information for all processes.
  /// \remark Might be "a bit racy" on 32 bit systems. Reading the values might
  /// return a intermediate result. Consult <a
  /// href="http://man7.org/linux/man-pages/man5/proc.5.html">proc doc</a> for
  /// more information.
  ///
  [[nodiscard]] size_t disk_read() const;

  ///
  /// \brief Get bytes written by this process
  /// \return Number of bytes this process caused to sent to the storage layer
  /// \note This value is only available for processes started by the current
  /// user. Run Moserep as root to information for all processes.
  /// \remark Might be "a bit racy" on 32 bit systems. Reading the values might
  /// return a intermediate result. Consult <a
  /// href="http://man7.org/linux/man-pages/man5/proc.5.html">proc doc</a> for
  /// more information.
  ///
  [[nodiscard]] size_t disk_write() const;

 private:
  size_t pid_;
  size_t virtual_memory_{0};
  size_t anonymous_memory_{0};
  size_t disk_read_{0};
  size_t disk_write_{0};
  std::string cmd_line_{""};
  std::string name_{""};
};

}  // namespace datamodel

}  // namespace reporter

#endif  // PROCESSINFORMATION_HPP
