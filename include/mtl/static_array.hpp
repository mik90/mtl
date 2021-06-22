#pragma once

#include <cstring>
#include <initializer_list>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>

#include "mtl/algorithm.hpp"
#include "mtl/iterator.hpp"
#include "mtl/maybe.hpp"

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

        std::for_each(begin, end,
                      [this](const auto& item) { return this->data_[this->size_++] = item; });
    }

    size_t size() const noexcept { return size_; }
    size_t capacity() const noexcept { return capacity_; }
    bool is_empty() const noexcept { return size_ == 0; }
    bool has_values() const noexcept { return !is_empty(); }

    ValueType* data() noexcept { return data_; }
    const ValueType* data() const noexcept { return data_; }

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
    // dumb iterators
    ValueType* begin() noexcept { return data_; }
    ValueType* end() noexcept { return data_ + size_; }
    const ValueType* begin() const noexcept { return cbegin(); }
    const ValueType* end() const noexcept { return cend(); }
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
        // Capacity should never be greater than size_, but the compiler warns about the possiblity
        if (idx >= size_ || idx >= capacity_) {
            return None{};
        }

        return Some{data_[idx]};
    }

    // Simply resets size, doesn't destroy anything
    void reset() noexcept { size_ = 0; }

    Maybe<ValueType> remove_at(size_t idx) {
        // Capacity should never be greater than size_, but the compiler warns about the possiblity
        if (idx >= size_ || idx >= capacity_) {
            return None{};
        }
        const auto ret_value = std::move(data_[idx]);
        // shift all values to the left
        // Overwrite old values
        /*
         * size = 4
         * idx == 1
         * values [0,  1,  2,  3]
         * idx    [0,  1,  2,  3]
         *         |   |   |---|
         *         |   |   |
         *         |   |   | move range to the left by 1 idx
         *         |   |
         *         |   | overwritten
         *         |
         *         | stays the same
         */
        if (idx < size() - 1) { // e.g. idx is 2 or less with size as 4
            // Some data still exists on the right of idx and needs to be shifted to the left
            const auto deleted_item_ptr = &data_[idx];
            const auto start_of_range_to_move = &data_[idx + 1];
            const auto n_bytes_to_move =
                sizeof(ValueType) * static_cast<std::size_t>(end() - start_of_range_to_move);
            std::memmove(deleted_item_ptr, start_of_range_to_move, n_bytes_to_move);
        } else {
            // Idx was at the end of the array so no data needs to be shifted over
        }
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
    std::initializer_list<ValueType> list = {std::forward<Args>(args)...};
    return StaticArray<ValueType, count>(list);
}

} // namespace mtl