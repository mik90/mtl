#pragma once

#include "mtl/maybe.hpp"
#include <iterator>
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
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = const ValueType&;
    using pointer = const ValueType*;

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

    // Prefix
    ConstIterator& operator++() {
        ++cur_;
        return *this;
    }

    // Postfix
    ConstIterator operator++(int) {
        auto temp = *this;
        ++*this;
        return temp;
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
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using reference = ValueType&;
    using pointer = ValueType*;

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
    // Prefix
    Iterator& operator++() {
        ++cur_;
        return *this;
    }

    // Postfix
    Iterator operator++(int) {
        auto temp = *this;
        ++*this;
        return temp;
    }

    Maybe<ValueType> copy_value() const {
        if (not_null()) {
            return *cur_;
        } else {
            return None{};
        }
    }

    ValueType& operator*() const { return *cur_; }
    bool is_null() const noexcept { return cur_ == nullptr; }
    bool not_null() const noexcept { return !is_null(); }
};
} // namespace mtl