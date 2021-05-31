#pragma once

#include <type_traits>
#include <utility>

namespace mtl {
/**
 * @brief similar to std::optional
 */
template <typename T>
class maybe {
  private:
    // What if `T` doesn't have a default constructor?
    T some_;
    enum class tag { SOME, NONE } tag_;
    maybe(tag tag_override) : tag_(tag_override) {}

  public:
    /// @todo There should be an easier way to return none
    static maybe none() { return maybe(tag::NONE); }

    /// @brief default ctor will default construct T
    maybe() : tag_(tag::SOME) {}

    // The member function itself must be templated for it to be a universal reference
    // https://stackoverflow.com/a/30569606/15827495
    template <typename U>
    maybe(U&& value) : some_(std::forward<U>(value)), tag_(tag::SOME) {
        /// @todo What if T is constructable from U? That's desired in this situation
        /*
        static_assert(std::is_same<std::decay_t<U>, std::decay_t<T>>::value,
                      "Constructor type U must be the same as class type T");
        */
    }

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