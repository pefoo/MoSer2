#ifndef ANY_H
#define ANY_H

#include <memory>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>

namespace utility {
namespace datastructure {

///
/// \brief A wrapper class for any type.
/// \details This classes uses type erasure to hide the actual data type.
///
class Any {
 public:
  ///
  /// \brief Create a new instance
  /// \details Creation of any object holding the type Any is forbidden
  /// \param value The value to wrap
  ///
  template <typename T, typename = std::enable_if_t<
                            !std::is_same<Any, std::decay_t<T>>::value>>
  explicit Any(T&& value)
      : content_(std::make_unique<Holder<T>>(std::forward<T>(value))) {}
  ///
  /// \brief Copy constructor
  /// \param other The object to copy
  ///
  Any(const Any& other) : content_(other.content_->clone()) {}

  ///
  /// \brief Move constructor
  /// \param other The object to move
  ///
  Any(Any&& other) noexcept { swap(other); }

  ///
  /// \brief operator =
  /// \param other
  /// \return
  ///
  Any& operator=(Any other) {
    swap(other);
    return *this;
  }

  ///
  /// \brief Get the underlying type info
  /// \return The type info
  ///
  const std::type_info& type() const {
    return this->content_.get()->type_info();
  }

  ///
  /// \brief swap
  /// \param other
  ///
  void swap(Any& other) noexcept { content_.swap(other.content_); }

  ///
  /// \brief default destructor
  ///
  ~Any() = default;

  ///
  /// \brief Get the wrapped data
  /// \return The wrapped data
  ///
  template <typename T>
  T& get() {
    if (std::type_index(typeid(T)) == std::type_index(content_->type_info())) {
      return static_cast<Holder<T>&>(*content_).value_;
    }
    throw std::bad_cast();
  }

 private:
  struct Placeholder {
    virtual ~Placeholder() = default;
    virtual const std::type_info& type_info() const = 0;
    virtual std::unique_ptr<Placeholder> clone() = 0;
  };

  template <typename T>
  struct Holder : public Placeholder {
    template <typename U>
    explicit Holder(U&& value) : value_(std::forward<U>(value)) {}
    const std::type_info& type_info() const override { return typeid(T); }
    std::unique_ptr<Placeholder> clone() override {
      return std::make_unique<Holder<T>>(value_);
    }
    T value_;
  };

  std::unique_ptr<Placeholder> content_;
};

}  // namespace datastructure
}  // namespace utility

#endif  // ANY_H
