#pragma once

namespace mtl {
  template <typename T> class View {
  public:
    View() = delete;
    View(T& value) : value_(value) {}

    View<T>& operator=(const T& other) {
      value_ = other;
      return *this;
    }
    View<T>& operator=(const View<T>& other) {
      value_ = other.value_;
      return *this;
    }

  private:
    T& value_;
  };
}  // namespace mtl