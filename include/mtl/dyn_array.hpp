#pragma once

#include "mtl/iterator.hpp"
#include "mtl/maybe.hpp"
#include "mtl/pointers.hpp"
#include "mtl/types.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <new>
#include <utility>

namespace mtl {
/**
 * @brief Array that can grow and shrink
 */
template <typename ValueType>
class DynArray {
  private:
    static constexpr usize default_capacity_ = 12;
    usize size_ = 0;
    usize capacity_ = default_capacity_;
    OwningPtr<ValueType[]> data_;

    // Allocates a copy of data_
    OwningPtr<ValueType[]> copy_data(const DynArray& other) {
        auto copy_ptr = new ValueType[other.capacity()];
        std::memcpy(copy_ptr, other.data(), other.size() * sizeof(ValueType));
        return OwningPtr<ValueType[]>(copy_ptr);
    }

    // Allocates the memory needed for data_
    OwningPtr<ValueType[]> allocate_new() {
        auto raw_ptr = new ValueType[capacity_];
        return OwningPtr<ValueType[]>(raw_ptr);
    }

    /// @brief Private so that you must explicitly use `copy()` in order to copy a DynArray
    DynArray(const DynArray& other)
        : size_(other.size()), capacity_(other.capacity()), data_(copy_data(other)) {}

    DynArray(usize capacity) : size_(0), capacity_(capacity), data_(allocate_new()) {}

    void increase_capacity() { set_capacity(capacity_ * 2); }

  public:
    DynArray() : size_(0), capacity_(default_capacity_), data_(allocate_new()) {}
    static DynArray make_with_capacity(usize capacity) { return DynArray(capacity); }

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

    void set_capacity(usize new_capacity) {
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
        OwningPtr<ValueType[]> temp = std::move(data_);
        data_ = allocate_new();
        std::memmove(data_.get(), temp.get(), size_);
    }

    template <class... Args>
    void emplace_back(Args&&... args) {
        if (size_ >= capacity_) {
            set_capacity(capacity_ * 2);
        }
        new (&data_[size_]) ValueType(std::forward<Args>(args)...);
        ++size_;
    }

    void push_back(const ValueType& value) { emplace_back(value); }

    void push_back(ValueType&& value) { emplace_back(std::move(value)); }

    /// @brief fill entirety of the array with a given value
    void fill_with(const ValueType& value) {
        size_ = 0;
        while (size_ < capacity_) {
            data_[size_++] = value;
        }
    }

    /// @brief Where GeneratorFunc is a callable that returns a value
    template <typename GeneratorFunc>
    void fill_with_generator(GeneratorFunc&& func) {
        size_ = 0;
        while (size_ < capacity_) {
            data_[size_++] = func();
        }
    }

    ValueType& operator[](usize idx) { return data_[idx]; }

    const ValueType& operator[](usize idx) const noexcept { return data_[idx]; }

    Maybe<ValueType> maybe_copy_at(usize idx) const {
        if (idx > size_) {
            return None{};
        }

        return Some{data_[idx]};
    }

    usize capacity() const noexcept { return capacity_; }
    usize size() const noexcept { return size_; }
    bool is_empty() const noexcept { return size_ == 0; }
    bool has_values() const noexcept { return !is_empty(); }

    ValueType* data() noexcept { return data_.get(); }
    const ValueType* data() const noexcept { return data_.get(); }
    OwningPtr<ValueType[]> take_data() { return OwningPtr<ValueType[]>(data_.release()); }

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