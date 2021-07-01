
#pragma once

#include "mtl/maybe.hpp"
#include <type_traits>

namespace mtl {
/**
 * @brief Iterates over a single type.
 * @todo Do i need an entirely separate specialization for const types or can i handle them in here?
 *        - SFINAE seems like the normal way to handle specialiation with const vs non-const
 * @todo Is there any way to deal with lifetimes during compile-time?
 *       I could add a sense of reference counting to underlying ranges but
 *       That seems like a heavyweight route.
 */
template <class ValueType>
class ConstIterator {
  private:
    const ValueType* cur_;

  public:
    explicit ConstIterator(const ValueType* iterable) : cur_(iterable) {}
    explicit ConstIterator() : cur_(nullptr) {}

    /// @todo Create static factory template that calls .begin() on a container. similar to
    /// std::begin

    /// @brief Value can be discarded
    Maybe<ConstIterator> next() {
        if (!cur_ || !(cur_ + 1)) {
            return None{};
        } else {
            return ConstIterator{++cur_};
        }
    }

    Maybe<ValueType> copy_value() const {
        if (not_null()) {
            return *cur_;
        } else {
            return None{};
        }
    }
    const ValueType& operator*() const { return *cur_; }
    bool is_null() const noexcept { return cur_ == nullptr; }
    bool not_null() const noexcept { return !is_null(); }
};

template <class ValueType>
class Iterator {
  private:
    ValueType* cur_;

  public:
    explicit Iterator(ValueType* iterable) : cur_(iterable) {}
    explicit Iterator() : cur_(nullptr) {}

    template <class ContainerType>
    explicit Iterator(ContainerType& iterable) {
        if (iterable.size() == 0) {
            cur_ = nullptr;
        } else {
            cur_ = iterable.data();
        }
    }

    /// @todo Create static factory template that calls .begin() on a container. similar to
    /// std::begin

    /// @brief Value can be discarded
    Maybe<Iterator> next() {
        if (!cur_ || !(cur_ + 1)) {
            return None{};
        } else {
            return Iterator{++cur_};
        }
    }

    Maybe<ValueType> copy_value() const {
        if (not_null()) {
            return *cur_;
        } else {
            return None{};
        }
    }

    bool is_null() const noexcept { return cur_ == nullptr; }
    bool not_null() const noexcept { return !is_null(); }
};
} // namespace mtl