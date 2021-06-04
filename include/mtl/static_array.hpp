#pragma once

#include <cstring>
#include <initializer_list>
#include <iterator>
#include <utility>

#include "mtl/maybe.hpp"

namespace mtl {
/**
 * @brief statically sized array
 */
template <typename ValueType, size_t Capacity>
class StaticArray {
  public:
    StaticArray() {}
    StaticArray(std::initializer_list<ValueType> list) : size_(list.size()) {
        std::memmove(data_, list.begin(), size_);
    }
    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return capacity_; }

    /// @brief Add an element if there's room, otherwise disregard it
    void push_back(const ValueType& value) {
        if (size_ < capacity_) {
            data_[size_++] = value;
        }
    }
    void push_back(ValueType&& value) {
        if (size_ < capacity_) {
            data_[size_++] = std::move(value);
        }
    }

    ValueType& operator[](size_t idx) { return data_[idx]; }

    const ValueType& operator[](size_t idx) const noexcept { return data_[idx]; }

    Maybe<ValueType> maybe_copy_at(size_t idx) const {
        if (idx < size_) {
            ValueType value = data_[idx];
            return value;
        } else {
            return None{};
        }
    }

  private:
    const size_t capacity_ = Capacity;
    ValueType data_[Capacity];
    size_t size_ = 0;
};
} // namespace mtl