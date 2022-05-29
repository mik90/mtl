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
class GenericPointer {
  public:
    using type = std::remove_extent_t<T>;
    GenericPointer() = default;
    GenericPointer(GenericPointer&& other) noexcept;

    // Raw access
    type* get() noexcept { return ptr_; }
    const type* get() const noexcept { return ptr_; }
    Maybe<type*> maybe_get();
    Maybe<const type*> maybe_get() const;

    // Operators
    GenericPointer<T>& operator=(GenericPointer<T>&& other);
    GenericPointer<T>& operator=(type* other);

    // Views
    bool has_value() const noexcept { return ptr_ != nullptr; }
    bool is_null() const noexcept { return !has_value(); }
    operator bool() const noexcept { return has_value(); }

    // Modifiers

    // swap spec
    void swap(GenericPointer<T>&& other);
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
GenericPointer<T>::GenericPointer(GenericPointer<T>&& other) noexcept : ptr_(other.get()) {
    other = nullptr;
}

// Operators
template <class T>
GenericPointer<T>& GenericPointer<T>::operator=(GenericPointer<T>&& other) {
    if (this == &other) {
        return *this;
    }
    reset(other.get());
    other.reset();
    return *this;
}

template <class T>
GenericPointer<T>& GenericPointer<T>::operator=(type* other) {
    reset(other);
    return *this;
}

// Modifiers
template <class T>
void GenericPointer<T>::swap(GenericPointer<T>&& other) {
    const auto temp = other.get();
    other.reset(ptr_);
    ptr_ = temp;
}

template <class T>
void GenericPointer<T>::reset() {
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
void GenericPointer<T>::reset(type* ptr) {
    reset();
    ptr_ = ptr;
}

template <class T>
typename GenericPointer<T>::type* GenericPointer<T>::release() {
    return std::exchange(ptr_, nullptr);
}

// Accessors

template <class T>
Maybe<typename GenericPointer<T>::type*> GenericPointer<T>::maybe_get() {
    if (has_value()) {
        return ptr_;
    }
    return None{};
}
template <class T>
Maybe<const typename GenericPointer<T>::type*> GenericPointer<T>::maybe_get() const {
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
class OwningPtr : public detail::GenericPointer<T> {
  public:
    OwningPtr() = default;
    explicit OwningPtr(T* ptr);
    OwningPtr(std::nullptr_t);
    OwningPtr(OwningPtr<T>&& other);
    ~OwningPtr();

    // no copy!
    OwningPtr(OwningPtr<T>& other) = delete;
    OwningPtr(const OwningPtr<T>& other) = delete;
    OwningPtr& operator=(const OwningPtr<T>& other) = delete;
    OwningPtr& operator=(OwningPtr<T>& other) = delete;

    // move-assign
    OwningPtr& operator=(OwningPtr<T>&& other);
    OwningPtr& operator=(T* other);

    T& operator*() { return *detail::GenericPointer<T>::ptr_; }
    const T& operator*() const { return detail::GenericPointer<T>::ptr_; }
    T* operator->() { return *detail::GenericPointer<T>::ptr_; }
    const T* operator->() const { return detail::GenericPointer<T>::ptr_; }
};

// Ctors
template <class T>
OwningPtr<T>::OwningPtr(T* ptr) {
    detail::GenericPointer<T>::ptr_ = ptr;
}

template <class T>
OwningPtr<T>::OwningPtr(std::nullptr_t) {
    detail::GenericPointer<T>::ptr_ = nullptr;
}

template <class T>
OwningPtr<T>::OwningPtr(OwningPtr<T>&& other) : detail::GenericPointer<T>(std::move(other)) {}

// dtor
template <class T>
OwningPtr<T>::~OwningPtr() {
    detail::GenericPointer<T>::reset();
};

// move-assign
template <class T>
OwningPtr<T>& OwningPtr<T>::operator=(OwningPtr<T>&& other) {
    detail::GenericPointer<T>::operator=(std::move(other));
    return *this;
}
template <class T>
OwningPtr<T>& OwningPtr<T>::operator=(T* other) {
    detail::GenericPointer<T>::operator=(other);
    return *this;
}

/**
 * @brief Array specialization for OwningPtr
 */
template <class T>
class OwningPtr<T[]> : public detail::GenericPointer<T[]> {
  public:
    OwningPtr() = default;
    explicit OwningPtr(T* ptr);
    OwningPtr(std::nullptr_t);
    OwningPtr(OwningPtr<T[]>&& other);
    ~OwningPtr();

    // no copy!
    OwningPtr(OwningPtr<T[]>& other) = delete;
    OwningPtr(const OwningPtr<T[]>& other) = delete;
    OwningPtr& operator=(const OwningPtr<T[]>& other) = delete;
    OwningPtr& operator=(OwningPtr<T[]>& other) = delete;

    // move-assign
    OwningPtr& operator=(OwningPtr<T[]>&& other);
    OwningPtr& operator=(T* other);

    // Array lookup
    T& operator[](mtl::usize pos) { return detail::GenericPointer<T[]>::ptr_[pos]; }
    const T& operator[](mtl::usize pos) const { return detail::GenericPointer<T[]>::ptr_[pos]; }

    T& operator*() { return *detail::GenericPointer<T[]>::ptr_; }
    const T& operator*() const { return detail::GenericPointer<T[]>::ptr_; }
    T* operator->() { return *detail::GenericPointer<T[]>::ptr_; }
    const T* operator->() const { return detail::GenericPointer<T[]>::ptr_; }
};
// Ctors
template <class T>
OwningPtr<T[]>::OwningPtr(T* ptr) {
    detail::GenericPointer<T[]>::ptr_ = ptr;
}

template <class T>
OwningPtr<T[]>::OwningPtr(std::nullptr_t) {
    detail::GenericPointer<T[]>::ptr_ = nullptr;
}

template <class T>
OwningPtr<T[]>::OwningPtr(OwningPtr<T[]>&& other) : detail::GenericPointer<T[]>(std::move(other)) {}

// dtor
template <class T>
OwningPtr<T[]>::~OwningPtr() {
    detail::GenericPointer<T[]>::reset();
};

// move-assign
template <class T>
OwningPtr<T[]>& OwningPtr<T[]>::operator=(OwningPtr<T[]>&& other) {
    detail::GenericPointer<T[]>::operator=(std::move(other));
    return *this;
}

template <class T>
OwningPtr<T[]>& OwningPtr<T[]>::operator=(T* other) {
    detail::GenericPointer<T[]>::operator=(other);
    return *this;
}

/**
 * @brief Shared free function helpers
 */
template <typename T, class... Args>
OwningPtr<T> make_owned(Args&&... args) {
    T* value = new T(std::forward<Args>(args)...);
    return OwningPtr<T>(value);
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