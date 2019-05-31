#ifndef INPUTFILECONTENT_HPP
#define INPUTFILECONTENT_HPP

#include <string>

namespace imonitorplugin {

///
/// \brief The input file content.
/// \details Stores two snapshots of a file, read with a delay.
///
class InputFileContent {
 public:
  ///
  /// \brief Create new instance
  /// \param snapshot_1 The first snapshot
  /// \param snapshot_2 The second snapshot
  /// \param timestamp The timestamp of the first read
  /// \param delay The delay between the consecutive reads in seconds
  ///
  InputFileContent(int64_t timestamp = 0, std::string snapshot_1 = "",
                   std::string snapshot_2 = "", int delay = 1)
      : timestamp_(timestamp),
        snapshot_1_(snapshot_1),
        snapshot_2_(snapshot_2),
        delay_(delay) {}

  ///
  /// \brief Get the first snapshot
  /// \return The content of the file
  ///
  std::string snapshot_1() const { return this->snapshot_1_; }

  ///
  /// \brief Get the second snaphot
  /// \return The content of the file
  ///
  std::string snapshot_2() const { return this->snapshot_2_; }

  ///
  /// \brief Set the first snapshot
  /// \param snapshot_1 The content of the file
  ///
  void setSnapshot_1(const std::string &snapshot_1) {
    this->snapshot_1_ = snapshot_1;
  }

  ///
  /// \brief Set the second snapshot
  /// \param snapshot_2 The content of the file
  ///
  void setSnapshot_2(const std::string &snapshot_2) {
    this->snapshot_2_ = snapshot_2;
  }

  ///
  /// \brief Get the timestamp of this record pair
  /// \return The timestamp of the first read
  ///
  int64_t timestamp() const { return this->timestamp_; }

  ///
  /// \brief Get the delay between the consecutive reads
  /// \return The delay in seconds
  ///
  int delay() const { return this->delay_; }

 private:
  int64_t timestamp_;
  std::string snapshot_1_;
  std::string snapshot_2_;
  int delay_;
};

}  // namespace imonitorplugin

#endif  // INPUTFILECONTENT_HPP
