#pragma once

#include <cstddef>
#include <memory>
#include <mtl/maybe.hpp>

namespace mtl {
/**
 * @brief Move-only pointer wrapper. Similar to unique_ptr
 * @todo Ensure that T
 * @todo Re-impl std::default_delete
 * @todo Specialize for T[]
 * @todo Handle custom deleters
 */

template <class T>
class OwnedPtr {
  private:
    T* ptr_ = nullptr;

  public:
    OwnedPtr(OwnedPtr& other) = delete;
    OwnedPtr(const OwnedPtr& other) = delete;
    OwnedPtr& operator=(const OwnedPtr& other) = delete;
    OwnedPtr& operator=(OwnedPtr& other) = delete;

    OwnedPtr(OwnedPtr&& other) {
        ptr_ = other.get();
        other.set(nullptr);
    }
    OwnedPtr& operator=(OwnedPtr&& other) {
        ptr_ = other.get();
        other.set(nullptr);
    }
    OwnedPtr& operator=(T* other) { ptr_ = other; }
    T& operator*() { return *ptr_; }
    const T& operator*() const { return *ptr_; }

    Maybe<T*> maybe_get() const noexcept {
        if (has_value()) {
            return Some{ptr_};
        }
        return None{};
    }

    bool has_value() const noexcept { return ptr_ != nullptr; }
    bool is_null() const noexcept { return !has_value(); }
    operator bool() const noexcept { return has_value(); }
    void clear() {
        if (ptr_ != nullptr) {
            delete ptr_;
        }
    }
    void swap(OwnedPtr&& other) {
        const auto temp = other.get();
        other.set(ptr_);
        ptr_ = temp;
    }

    OwnedPtr() = default;
    OwnedPtr(T* ptr) : ptr_(ptr) {}
    OwnedPtr(std::nullptr_t) : ptr_(nullptr) {}
    ~OwnedPtr() { clear(); };

    void set(T* ptr) noexcept { ptr_ = ptr; }
    T* get() noexcept { return ptr_; }
    const T* get() const noexcept { return ptr_; }

    /// @brief Returns pointer, setting interval value to null
    T* release() {
        const auto temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }
};

template <class T>
class OwnedPtr<T[]> {
  private:
    T* ptr_ = nullptr;

  public:
    OwnedPtr(OwnedPtr& other) = delete;
    OwnedPtr(const OwnedPtr& other) = delete;
    OwnedPtr& operator=(const OwnedPtr& other) = delete;
    OwnedPtr& operator=(OwnedPtr& other) = delete;
    OwnedPtr(OwnedPtr&& other) {
        ptr_ = other.get();
        other.set(nullptr);
    }

    OwnedPtr& operator=(OwnedPtr&& other) {
        ptr_ = other.get();
        other.set(nullptr);
        return *this;
    }
    OwnedPtr& operator=(T* other) { ptr_ = other; }

    T& operator*() { return *ptr_; }

    const T& operator*() const { return *ptr_; }

    Maybe<T*> maybe_get() const noexcept {
        if (ptr_) {
            return Some{ptr_};
        }
        return None{};
    }

    bool has_value() const noexcept { return ptr_ != nullptr; }
    bool is_null() const noexcept { return !has_value(); }
    operator bool() const noexcept { return has_value(); }
    void clear() {
        if (ptr_ != nullptr) {
            delete[] ptr_;
        }
    }
    void swap(OwnedPtr&& other) {
        const auto temp = other.get();
        other.set(ptr_);
        ptr_ = temp;
    }
    T& operator[](size_t idx) { return ptr_[idx]; }

    const T& operator[](size_t idx) const noexcept { return ptr_[idx]; }

    OwnedPtr() = default;
    OwnedPtr(T* ptr) : ptr_(ptr) {}
    OwnedPtr(std::nullptr_t) : ptr_(nullptr) {}
    ~OwnedPtr() { clear(); };

    void set(T* ptr) noexcept { ptr_ = ptr; }
    T* get() noexcept { return ptr_; }
    const T* get() const noexcept { return ptr_; }
    T* release() {
        const auto temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }
};

template <typename T, class... Args>
static OwnedPtr<T> make_owned(Args&&... args) {
    T* value = new T(std::forward<Args>(args)...);
    return OwnedPtr<T>(value);
}

/**
 * @brief Copyable pointer wrapper. Similar to shared_ptr
 */
class SharedPtr {};

/**
 * @brief Copyable pointer wrapper. Similar to weak_ptr
 */
class NonOwnedPtr {};
} // namespace mtl