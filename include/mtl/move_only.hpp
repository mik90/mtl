#pragma once

#include <utility>

namespace mtl {

/**
 * @brief Stack-allocated wrapper that makes a type move-only
 */
template <typename T>
class MoveOnly {
  public:
    explicit MoveOnly(T&& value) : value_(std::forward<T>(value)) {}

    template <class... Args>
    MoveOnly(Args&&... args) : value_(std::forward<Args>(args)...) {}

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;

    // Accessors
    const T* operator->() const noexcept { return &value_; }
    const T& operator*() const noexcept { return value_; }

    // Modifiers
    T* operator->() noexcept { return &value_; }
    T& operator*() noexcept { return value_; }

    T&& release() noexcept { return std::move(value_); }

  private:
    T value_;
};

} // namespace mtl