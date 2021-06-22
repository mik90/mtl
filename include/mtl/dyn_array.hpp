#pragma once

#include "mtl/iterator.hpp"
#include "mtl/maybe.hpp"
#include "mtl/pointers.hpp"
#include <cstdlib>
#include <cstring>
#include <utility>

namespace mtl {
/**
 * @brief Array that can grow and shrink
 */
template <typename ValueType>
class DynArray {
  private:
    static constexpr std::size_t default_capacity_ = 12;
    std::size_t size_ = 0;
    std::size_t capacity_ = default_capacity_;
    OwnedPtr<ValueType[]> data_;

    // Allocates a copy of data_
    OwnedPtr<ValueType[]> copy_data(const DynArray& other) {
        auto copy_ptr = new ValueType[other.capacity()];
        std::memcpy(copy_ptr, other.data(), other.size() * sizeof(ValueType));
        return OwnedPtr<ValueType[]>(copy_ptr);
    }

    OwnedPtr<ValueType[]> allocate_new() {
        auto raw_ptr = new ValueType[capacity_];
        return OwnedPtr<ValueType[]>(raw_ptr);
    }

    /// @brief Private so that you must explicitly use `copy()` in order to copy a DynArray
    DynArray(const DynArray& other)
        : size_(other.size()), capacity_(other.capacity()), data_(copy_data(other)) {}

    DynArray(std::size_t capacity) : size_(0), capacity_(capacity), data_(allocate_new()) {}

  public:
    DynArray() : size_(0), capacity_(default_capacity_), data_(allocate_new()) {}
    static DynArray make_with_capacity(std::size_t capacity) { return DynArray(capacity); }

    DynArray copy() const { return DynArray(*this); }

    DynArray& operator=(const DynArray& other) {
        size_ = other.size();
        capacity_ = other.capacity();
        data_ = copy_data(other);
        return *this;
    }

    DynArray& operator=(DynArray&& other) {
        size_ = other.size();
        capacity_ = other.capacity();
        data_ = other.take_data();
        return *this;
    }

    void set_capacity(std::size_t new_capacity) {
        if (new_capacity == capacity_) {
            // No change
            return;
        }
        if (new_capacity < size_) {
            // New capacity is smol-er
            size_ = new_capacity;
        }
        // Update capacity
        capacity_ = new_capacity;
        OwnedPtr<ValueType[]> temp = std::move(data_);
        data_ = allocate_new();
        std::memmove(data_.get(), temp.get(), size_);
    }

    DynArray(std::initializer_list<ValueType> list) : size_(0), capacity_(default_capacity_) {
        data_ = allocate_new();

        auto begin = list.begin();
        auto end = list.end();
        // Limit the amount of values copied over if it's greater than the capacity
        if (list.size() > capacity_) {
            set_capacity(list.size());
        }

        std::for_each(begin, end,
                      [this](const auto& item) { return this->data_[this->size_++] = item; });
    }

    void push_back(const ValueType& value) {
        if (size_ >= capacity_) {
            set_capacity(capacity_ * 2);
        }
        data_[size_++] = value;
    }
    void push_back(ValueType&& value) {
        if (size_ >= capacity_) {
            set_capacity(capacity_ * 2);
        }
        data_[size_++] = std::move(value);
    }

    ValueType& operator[](size_t idx) { return data_[idx]; }

    const ValueType& operator[](size_t idx) const noexcept { return data_[idx]; }

    Maybe<ValueType> maybe_copy_at(size_t idx) const {
        if (idx > size_) {
            return None{};
        }

        return Some{data_[idx]};
    }

    std::size_t capacity() const noexcept { return capacity_; }
    std::size_t size() const noexcept { return size_; }
    bool is_empty() const noexcept { return size_ == 0; }
    bool has_values() const noexcept { return !is_empty(); }

    ValueType* data() noexcept { return data_.get(); }
    const ValueType* data() const noexcept { return data_.get(); }
    OwnedPtr<ValueType[]> take_data() { return OwnedPtr<ValueType[]>(data_.release()); }

    Iterator<ValueType> iter() {
        if (size() > 0) {
            return Iterator(data());
        } else {
            return Iterator<ValueType>();
        }
    }

    const ConstIterator<ValueType> c_iter() const {
        if (size() > 0) {
            return ConstIterator(data());
        } else {
            return ConstIterator<ValueType>();
        }
    }

    // Dumb iterators
    ValueType* begin() noexcept { return data_.get(); }
    ValueType* end() noexcept { return data_.get() + size_; }
    const ValueType* begin() const noexcept { return cbegin(); }
    const ValueType* end() const noexcept { return cbegin(); }
    const ValueType* cbegin() const noexcept { return data_.get(); }
    const ValueType* cend() const noexcept { return data_.get() + size_; }
};
} // namespace mtl