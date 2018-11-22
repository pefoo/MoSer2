#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <stddef.h>
#include <mutex>
#include <vector>
// TODO using vector as base might not be that smart since the whole buffer is
// allocated when creating this class
namespace utility {
namespace datastructure {
///
/// \brief A circular buffer
///
template <typename T>
class CircularBuffer {
 public:
  ///
  /// \brief Create a new buffer
  /// \param max_size The max size of the buffer
  ///
  explicit CircularBuffer(size_t max_size)
      : max_size_(max_size), data_(std::vector<T>(max_size)) {}

  ///
  /// \brief Put a new item
  /// \param item The item to insert
  ///
  void Put(T item) {
    std::lock_guard<std::mutex> lock(this->mutex_);
    this->data_[this->head_] = item;
    if (this->is_full()) {
      this->tail_ = (this->tail_ + 1) % this->max_size_;
    }
    this->head_ = (this->head_ + 1) % this->max_size_;
    this->full_ = this->head_ == this->tail_;
  }

  ///
  /// \brief Get the next item
  /// \param item The next item (out parameter)
  /// \return True, if the buffer was not empty
  ///
  bool Get(T* item) {
    std::lock_guard<std::mutex> lock(this->mutex_);
    if (this->is_empty()) {
      return false;
    }
    *item = this->data_[this->tail_];
    this->full_ = false;
    this->tail_ = (this->tail_ + 1) % this->max_size_;
    return true;
  }

  ///
  /// \brief Reset the buffer, clearing all entries
  ///
  void Reset() {
    this->data_.clear();
    this->data_.resize(this->max_size_);
    this->head_ = this->tail_ = 0;
    this->full_ = false;
  }

  ///
  /// \brief Get the current size of the buffer
  /// \return The size of the buffer
  ///
  size_t size() const {
    size_t s = this->max_size_;
    if (!this->is_full()) {
      if (this->head_ >= this->tail_) {
        s = this->head_ - this->tail_;
      } else {
        s = this->max_size_ + this->head_ - this->tail_;
      }
    }
    return s;
  }

  ///
  /// \brief is_empty
  /// \return True, if the buffer is empty
  ///
  bool is_empty() const {
    return !this->is_full() && this->head_ == this->tail_;
  }

  ///
  /// \brief is_full
  /// \return True, if the buffer is full
  ///
  bool is_full() const { return this->full_; }

 private:
  const size_t max_size_;
  std::mutex mutex_;
  std::vector<T> data_;
  size_t head_ = 0;
  size_t tail_ = 0;
  bool full_ = false;
};
}  // namespace datastructure
}  // namespace utility
#endif  // CIRCULARBUFFER_H
