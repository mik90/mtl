#pragma once

#include <initializer_list>
#include <iterator>
#include <utility>
#include <type_traits>

#include "mtl/maybe.hpp"

namespace mtl {

/**
 * @brief statically sized array
 */
template <typename ValueType, size_t Capacity>
class StaticArray {
  public:
    StaticArray() {}

    /// @note User is expected to make sure that Capacity > list.size()
    StaticArray(std::initializer_list<ValueType> list) : size_(0) {
        auto end = list.end();
        // Limit the amount of values copied over if it's greater than the capacity
        if (list.size() > Capacity) {
            end = list.begin() + Capacity;
        }

        std::for_each(list.begin(), end, [this](const auto& item){
            this->data_[this->size_++] = item;
        });
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
        if (idx > size_) {
            return None{};
        }

        return data_[idx];
    }

    // Simply resets size, doesn't destroy anything
    void reset() noexcept {
        size_ = 0;
    }

    Maybe<ValueType> remove_at(size_t idx) {
        if (idx > size_) {
            return None{};
        }
        const auto ret_value = data_[idx];
        /// @todo impl this
    }

  private:
    const size_t capacity_ = Capacity;
    ValueType data_[Capacity];
    size_t size_ = 0;
};
} // namespace mtl