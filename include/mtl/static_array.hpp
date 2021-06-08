#pragma once

#include <cstring>
#include <initializer_list>
#include <iterator>
#include <utility>
#include <type_traits>

#include "mtl/maybe.hpp"
#include "mtl/algorithm.hpp"

namespace mtl {

/**
 * @brief Array with a static capacity and variable size
 */
template <typename ValueType, size_t Capacity>
class StaticArray {
  public:
    StaticArray() {}

    /// @note User is expected to make sure that Capacity > list.size()
    StaticArray(std::initializer_list<ValueType> list) : size_(0) {
        auto begin = list.begin();
        auto end = list.end();
        // Limit the amount of values copied over if it's greater than the capacity
        if (list.size() > Capacity) {
            end = list.begin() + Capacity;
        }

        std::for_each(begin, end, [this](const auto& item){
            return this->data_[this->size_++] = item;
        });
    }

    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return capacity_; }

    // Iterators
    ValueType* begin() noexcept { return data_; }
    ValueType* end() noexcept { return data_ + size_; }
    const ValueType* cbegin() const noexcept { return data_; }
    const ValueType* cend() const noexcept { return data_ + size_; }

    /// @brief Add an element if there's room, otherwise disregard it and return false
    bool push_back(const ValueType& value) {
        if (size_ < capacity_) {
            data_[size_++] = value;
            return true;
        }
        return false;
    }
    bool push_back(ValueType&& value) {
        if (size_ < capacity_) {
            data_[size_++] = std::move(value);
            return true;
        }
        return false;
    }

    ValueType& operator[](size_t idx) { return data_[idx]; }

    const ValueType& operator[](size_t idx) const noexcept { return data_[idx]; }

    Maybe<ValueType> maybe_copy_at(size_t idx) const {
        if (idx > size_) {
            return None{};
        }

        return Some{data_[idx]};
    }

    // Simply resets size, doesn't destroy anything
    void reset() noexcept {
        size_ = 0;
    }

    Maybe<ValueType> remove_at(size_t idx) {
        if (idx > size_) {
            return None{};
        }
        const auto ret_value = std::move(data_[idx]);
        // range to shift
        const auto n_values_to_shift = end() - idx - 1;
        // Overwrite old values
        std::memmove(begin() + index, begin() + index + 1, n_values_to_shift);
        size_--;
        return Some{ret_value};
    }

  private:
    const size_t capacity_ = Capacity;
    ValueType data_[Capacity];
    size_t size_ = 0;
};

/**
 * @brief Converts variadic args into a given ValueType
 * stackoverflow answer describing std::make_array impl: https://stackoverflow.com/a/39040524
 * There's probably a better way to do this than to make an initializer_list
 */
template <typename ValueType, class... Args>
static auto make_static_array(Args&&... args) {
    const auto count = sizeof...(Args);
    std::initializer_list<ValueType> list = { std::forward<Args>(args)... };
    return StaticArray<ValueType, count>(list);
}


} // namespace mtl