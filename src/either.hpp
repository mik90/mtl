#pragma once

/**
 * @brief Similar to std::variant but only for two types. More generic than mtl::result
 * Uses a tagged union internally. Not thread-safe.
 */

namespace mtl {
template <typename FirstType, typename SecondType>
class either {
  private:
    enum class tag { FIRST, SECOND, NONE } tag_;
    union {
        FirstType first_;
        SecondType second_;
    };

    void clear_tag() noexcept { tag_ = tag::NONE; };

  public:
    explicit either(FirstType&& first) : tag_(tag::FIRST), first_(std::forward<FirstType>(first)) {}

    explicit either(SecondType&& second)
        : tag_(tag::SECOND), second_(std::forward<SecondType>(second)) {}

    either() : tag_(tag::NONE) {}

    /// @todo Only define destructor if FirstType or SecondType types are non-trivial
    ~either() {}

    // Observers
    bool has_first() const noexcept { return tag_ == tag::FIRST; }

    bool has_second() const noexcept { return tag_ == tag::SECOND; }

    bool has_any() const noexcept { return tag_ != tag::NONE; }

    // Borrowers
    FirstType& get_first() { return first_; }

    const FirstType& get_first() const { return first_; }

    SecondType& get_second() { return second_; }

    const SecondType& get_second() const { return second_; }

    // Ownership modifiers
    FirstType&& release_first() {
        clear_tag();
        return std::move(first_);
    }

    FirstType&& release_first_or(FirstType default_val) {
        if (has_first()) {
            clear_tag();
            return release_first();
        } else {
            return default_val;
        }
    }

    SecondType&& release_second() {
        clear_tag();
        return std::move(second_);
    }

    FirstType&& release_second_or(FirstType default_val) {
        if (has_second()) {
            clear_tag();
            return release_second();
        } else {
            return default_val;
        }
    }
};
} // namespace mtl