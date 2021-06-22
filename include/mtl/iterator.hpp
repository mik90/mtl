
#pragma once

#include "mtl/maybe.hpp"

namespace mtl {
/**
 * @brief Iterates over a single type
 */
template <class ValueType>
class Iterator {
  private:
    ValueType* cur_;

  public:
    Iterator(ValueType* iterable) : cur_(iterable) {}

    Maybe<ValueType> next() {
        if (!cur_ || !(cur_ + 1)) {
            return None{};
        } else {
            return ++cur_;
        }
    }

    bool is_null() const noexcept { return cur_ == nullptr; }
    bool not_null() const noexcept { return !is_null(); }
};
} // namespace mtl