#pragma once

#include <cstddef>
#include <memory>
#include <mtl/maybe.hpp>
#include <mtl/types.hpp>
#include <type_traits>

namespace mtl {
/**
 * @brief Common pointer abstractions
 * @todo Handle custom deleters
 * Generalize common parts, ripped off <https://codereview.stackexchange.com/a/263999>
 */
namespace detail {
template <class T>
class BaseOwningPtr {
  public:
    using type = std::remove_extent_t<T>;
    BaseOwningPtr() = default;
    BaseOwningPtr(type* ptr);
    BaseOwningPtr(BaseOwningPtr&& other) noexcept;
    ~BaseOwningPtr();

    // Raw access
    type* get() noexcept { return ptr_; }
    const type* get() const noexcept { return ptr_; }
    Maybe<type*> maybe_get();
    Maybe<const type*> maybe_get() const;

    // Operators
    BaseOwningPtr<T>& operator=(BaseOwningPtr<T>&& other);
    BaseOwningPtr<T>& operator=(type* other);

    // Views
    bool has_value() const noexcept { return ptr_ != nullptr; }
    bool is_null() const noexcept { return !has_value(); }
    operator bool() const noexcept { return has_value(); }

    // Modifiers

    // swap spec
    void swap(BaseOwningPtr<T>&& other);
    // Delete current data
    void reset();
    // Delete current data and use new data provided as arg
    void reset(type* ptr);
    // take ownership of raw ptr
    type* release();

  protected:
    type* ptr_ = nullptr;
};

// Ctors
template <class T>
BaseOwningPtr<T>::BaseOwningPtr(type* ptr) : ptr_(ptr) {}

template <class T>
BaseOwningPtr<T>::BaseOwningPtr(BaseOwningPtr<T>&& other) noexcept : ptr_(other.release()) {}

// Dtor
template <class T>
BaseOwningPtr<T>::~BaseOwningPtr() {
    reset();
}

// Operators
template <class T>
BaseOwningPtr<T>& BaseOwningPtr<T>::operator=(BaseOwningPtr<T>&& other) {
    if (this == &other) {
        return *this;
    }
    // Delete our pointer
    reset();
    // Take ownership of the other's pointer
    ptr_ = other.release();
    // AH, reset current ptr
    return *this;
}

template <class T>
BaseOwningPtr<T>& BaseOwningPtr<T>::operator=(type* other) {
    // Delete our pointer
    reset();
    // Take the other's
    ptr_ = other;
    return *this;
}

// Modifiers
template <class T>
void BaseOwningPtr<T>::swap(BaseOwningPtr<T>&& other) {
    const auto ourOldPtr = ptr_;
    ptr_ = other.get();

    other.ptr_ = ourOldPtr;
}

template <class T>
void BaseOwningPtr<T>::reset() {
    if (has_value()) {
        if (std::is_array_v<T>) {
            delete[] ptr_;
        } else {
            delete ptr_;
        }
    }
    ptr_ = nullptr;
}

template <class T>
void BaseOwningPtr<T>::reset(type* ptr) {
    reset();
    ptr_ = ptr;
}

template <class T>
typename BaseOwningPtr<T>::type* BaseOwningPtr<T>::release() {
    return std::exchange(ptr_, nullptr);
}

// Accessors

template <class T>
Maybe<typename BaseOwningPtr<T>::type*> BaseOwningPtr<T>::maybe_get() {
    if (has_value()) {
        return ptr_;
    }
    return None{};
}
template <class T>
Maybe<const typename BaseOwningPtr<T>::type*> BaseOwningPtr<T>::maybe_get() const {
    if (has_value()) {
        return ptr_;
    }
    return None{};
}

} // namespace detail

/**
 * @brief Move-only pointer, similar to std::unique_ptr
 */
template <class T>
class OwningPtr : public detail::BaseOwningPtr<T> {
  public:
    OwningPtr() = default;
    explicit OwningPtr(T* ptr);
    OwningPtr(std::nullptr_t);
    OwningPtr(OwningPtr<T>&& other);

    // no copy!
    OwningPtr(OwningPtr<T>& other) = delete;
    OwningPtr(const OwningPtr<T>& other) = delete;
    OwningPtr& operator=(const OwningPtr<T>& other) = delete;
    OwningPtr& operator=(OwningPtr<T>& other) = delete;

    // move-assign
    OwningPtr& operator=(OwningPtr<T>&& other);
    OwningPtr& operator=(T* other);

    T& operator*() { return *detail::BaseOwningPtr<T>::ptr_; }
    const T& operator*() const { return detail::BaseOwningPtr<T>::ptr_; }
    T* operator->() { return *detail::BaseOwningPtr<T>::ptr_; }
    const T* operator->() const { return detail::BaseOwningPtr<T>::ptr_; }
};

// Ctors
template <class T>
OwningPtr<T>::OwningPtr(T* ptr) : detail::BaseOwningPtr<T>(ptr) {}

template <class T>
OwningPtr<T>::OwningPtr(std::nullptr_t) {
    detail::BaseOwningPtr<T>::ptr_ = nullptr;
}

template <class T>
OwningPtr<T>::OwningPtr(OwningPtr<T>&& other) : detail::BaseOwningPtr<T>(std::move(other)) {}

// move-assign
template <class T>
OwningPtr<T>& OwningPtr<T>::operator=(OwningPtr<T>&& other) {
    detail::BaseOwningPtr<T>::operator=(std::move(other));
    return *this;
}

template <class T>
OwningPtr<T>& OwningPtr<T>::operator=(T* other) {
    detail::BaseOwningPtr<T>::operator=(other);
    return *this;
}

/**
 * @brief Array specialization for OwningPtr
 */
template <class T>
class OwningPtr<T[]> : public detail::BaseOwningPtr<T[]> {
  public:
    OwningPtr() = default;
    explicit OwningPtr(T* ptr);
    OwningPtr(std::nullptr_t);
    OwningPtr(OwningPtr<T[]>&& other);

    // no copy!
    OwningPtr(OwningPtr<T[]>& other) = delete;
    OwningPtr(const OwningPtr<T[]>& other) = delete;
    OwningPtr& operator=(const OwningPtr<T[]>& other) = delete;
    OwningPtr& operator=(OwningPtr<T[]>& other) = delete;

    // move-assign
    OwningPtr& operator=(OwningPtr<T[]>&& other);
    OwningPtr& operator=(T* other);

    // Array lookup
    T& operator[](mtl::usize pos) { return detail::BaseOwningPtr<T[]>::ptr_[pos]; }
    const T& operator[](mtl::usize pos) const { return detail::BaseOwningPtr<T[]>::ptr_[pos]; }

    T& operator*() { return *detail::BaseOwningPtr<T[]>::ptr_; }
    const T& operator*() const { return detail::BaseOwningPtr<T[]>::ptr_; }
    T* operator->() { return *detail::BaseOwningPtr<T[]>::ptr_; }
    const T* operator->() const { return detail::BaseOwningPtr<T[]>::ptr_; }
};
// Ctors
template <class T>
OwningPtr<T[]>::OwningPtr(T* ptr) : detail::BaseOwningPtr<T[]>(ptr) {}

template <class T>
OwningPtr<T[]>::OwningPtr(std::nullptr_t) {
    detail::BaseOwningPtr<T[]>::ptr_ = nullptr;
}

template <class T>
OwningPtr<T[]>::OwningPtr(OwningPtr<T[]>&& other) : detail::BaseOwningPtr<T[]>(std::move(other)) {}

// move-assign
template <class T>
OwningPtr<T[]>& OwningPtr<T[]>::operator=(OwningPtr<T[]>&& other) {
    detail::BaseOwningPtr<T[]>::operator=(std::move(other));
    return *this;
}

template <class T>
OwningPtr<T[]>& OwningPtr<T[]>::operator=(T* other) {
    detail::BaseOwningPtr<T[]>::operator=(other);
    return *this;
}

/**
 * @brief Shared free function helpers
 */
template <typename T, class... Args>
OwningPtr<T> make_owned(Args&&... args) {
    return OwningPtr<T>(new T(std::forward<Args>(args)...));
}

/**
 * @brief Copyable pointer wrapper. Similar to shared_ptr
 */
class SharedPtr {};

/**
 * @brief Copyable pointer wrapper. Similar to weak_ptr
 */
class NonOwningPtr {};
} // namespace mtl