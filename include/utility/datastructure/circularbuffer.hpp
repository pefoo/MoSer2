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
      : max_size(max_size), data(std::vector<T>(max_size)) {}

  ///
  /// \brief Put a new item
  /// \param item The item to insert
  ///
  void Put(T item) {
    std::lock_guard<std::mutex> lock(this->mutex);
    this->data[this->head] = item;
    if (this->is_full()) {
      this->tail = (this->tail + 1) % this->max_size;
    }
    this->head = (this->head + 1) % this->max_size;
    this->full = this->head == this->tail;
  }

  ///
  /// \brief Get the next item
  /// \param item The next item (out parameter)
  /// \return True, if the buffer was not empty
  ///
  bool Get(T* item) {
    std::lock_guard<std::mutex> lock(this->mutex);
    if (this->is_empty()) {
      return false;
    }
    *item = this->data[this->tail];
    this->full = false;
    this->tail = (this->tail + 1) % this->max_size;
    return true;
  }

  ///
  /// \brief Reset the buffer, clearing all entries
  ///
  void Reset() {
    this->data.clear();
    this->data.resize(this->max_size);
    this->head = this->tail = 0;
    this->full = false;
  }

  ///
  /// \brief Get the current size of the buffer
  /// \return The size of the buffer
  ///
  size_t size() const {
    size_t s = this->max_size;
    if (!this->is_full()) {
      if (this->head >= this->tail) {
        s = this->head - this->tail;
      } else {
        s = this->max_size + this->head - this->tail;
      }
    }
    return s;
  }

  ///
  /// \brief is_empty
  /// \return True, if the buffer is empty
  ///
  bool is_empty() const { return !this->is_full() && this->head == this->tail; }

  ///
  /// \brief is_full
  /// \return True, if the buffer is full
  ///
  bool is_full() const { return this->full; }

 private:
  const size_t max_size;
  std::mutex mutex;
  std::vector<T> data;
  size_t head = 0;
  size_t tail = 0;
  bool full = false;
};
}  // namespace datastructure
}  // namespace utility
#endif  // CIRCULARBUFFER_H
