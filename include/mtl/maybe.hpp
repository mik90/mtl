#pragma once

#include <type_traits>
#include <utility>

namespace mtl {
struct None {};

template <typename T>
struct Some {
    T value_;
};

/**
 * @brief similar to std::optional
 */
template <typename T>
class Maybe {
  private:
    // Unions cannot have data members of reference types
    union {
        T some_;
        None none_;
    };
    enum class tag { SOME, NONE } tag_;
    Maybe(tag tag_override) : none_(None{}), tag_(tag_override) {}

  public:
    static Maybe none() { return Maybe(tag::NONE); }
    Maybe(None) : none_(None{}), tag_(tag::NONE) {}

    // Construct from Some
    Maybe(const Some<T>& some) : some_(some.value_), tag_(tag::SOME) {}
    Maybe(Some<T>&& some) : some_(some.value_), tag_(tag::SOME) {}

    // The member function itself must be templated for it to be a universal reference
    // https://stackoverflow.com/a/30569606/15827495
    template <typename U>
    Maybe(U&& value) : some_(std::forward<U>(value)), tag_(tag::SOME) {}

    /// @todo only define this if T is not trivially destructable
    ~Maybe() {}

    // Observers
    bool is_some() const noexcept { return tag_ == tag::SOME; }
    bool is_none() const noexcept { return !is_some(); }
    const T& get_some() const noexcept { return some_; }

    // Modifiers
    /// @todo Should this throw or just be undefined in case there's none?
    T& get_some() { return some_; }
    T& get_some_or(T default_value) {
        if (tag_ == tag::SOME) {
            return some_;
        } else {
            return default_value;
        }
    }

    // Ownership modifiers
    T&& release() {
        tag_ = tag::NONE;
        return std::move(some_);
    }
    T&& release_or(T&& default_value) {
        if (tag_ == tag::SOME) {
            tag_ = tag::NONE;
            return std::move(some_);
        } else {
            return std::move(default_value);
        }
    }
};
} // namespace mtl