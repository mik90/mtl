#pragma once

#include "mtl/maybe.hpp"
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
    static constexpr std::size_t default_alignment_ = sizeof(std::size_t);
    // I'd need OwnedPtr<T[]> to use that, i think
    ValueType* data_;
    std::size_t size_ = 0;
    std::size_t capacity_ = default_capacity_;

  public:
    DynArray() {
        data_ = static_cast<ValueType*>(
            std::aligned_alloc(default_alignment_, default_capacity_ * sizeof(ValueType)));
    }

    void set_capacity(std::size_t new_capacity) {
        if (new_capacity == capacity_) {
            return;
        }
        if (new_capacity < capacity_) {
            // New capacity is smol-er
            size_ = capacity_;
        }
        // Increase capacity
        capacity_ = new_capacity;
        const ValueType* temp = data_;
        data_ = static_cast<ValueType*>(
            std::aligned_alloc(default_alignment_, capacity_ * sizeof(ValueType)));
        std::memmove(data_, temp, size_);
    }

    DynArray(std::initializer_list<ValueType> list) {
        data_ = static_cast<ValueType*>(
            std::aligned_alloc(default_alignment_, default_capacity_ * sizeof(ValueType)));

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
};
} // namespace mtl