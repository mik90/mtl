#pragma once

#include <initializer_list>
#include <iterator>
#include <utility>

namespace mtl {
/**
 * @brief statically sized array
 */
template <typename ValueType, size_t Capacity>
class StaticArray {
  public:
    StaticArray() {}
    StaticArray(std::initializer_list<ValueType> list) : size_(list.size()) {
        for (size_t i = 0; i < list.size(); ++i) {
            data_[i] = *(list.begin() + i);
        }
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
    // TODO operator[]

  private:
    const size_t capacity_ = Capacity;
    ValueType data_[Capacity];
    size_t size_ = 0;
};
} // namespace mtl