#ifndef INPUTFILECONTENT_HPP
#define INPUTFILECONTENT_HPP

#include <string>
#include <utility>

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
  explicit InputFileContent(int64_t timestamp_1 = 0,
                            std::string snapshot_1 = "",
                            std::string snapshot_2 = "",
                            int64_t timestamp_2 = 0, int delay = 1)
      : timestamp_1_(timestamp_1),
        timestamp_2_(timestamp_2),
        snapshot_1_(std::move(snapshot_1)),
        snapshot_2_(std::move(snapshot_2)),
        delay_(delay) {}

  ///
  /// \brief Get the first snapshot
  /// \return The content of the file
  ///
  [[nodiscard]] std::string snapshot_1() const { return this->snapshot_1_; }

  ///
  /// \brief Get the second snaphot
  /// \return The content of the file
  ///
  [[nodiscard]] std::string snapshot_2() const { return this->snapshot_2_; }

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
  /// \brief Get the timestamp of the first snapshot
  /// \return The timestamp of the first read
  ///
  [[nodiscard]] int64_t timestamp_1() const { return this->timestamp_1_; }

  ///
  /// \brief Get the timestamp of the second snapshot
  /// \return The timestamp of the second read
  ///
  [[nodiscard]] int64_t timestamp_2() const { return this->timestamp_2_; }

  ///
  /// \brief Set timestamp of the first snapshot
  /// \param timestamp_2 The first timestamp
  ///
  void setTimestamp_1(const int64_t &timestamp_1);

  ///
  /// \brief Set timestamp of the second snapshot
  /// \param timestamp_2 The second timestamp
  ///
  void setTimestamp_2(const int64_t &timestamp_2) {
    this->timestamp_2_ = timestamp_2;
  }

  ///
  /// \brief Get the delay between the consecutive reads
  /// \return The delay in seconds
  ///
  [[nodiscard]] int delay() const { return this->delay_; }

 private:
  int64_t timestamp_1_;
  int64_t timestamp_2_;
  std::string snapshot_1_;
  std::string snapshot_2_;
  int delay_;
};

}  // namespace imonitorplugin

#endif  // INPUTFILECONTENT_HPP
