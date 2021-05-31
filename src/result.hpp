#pragma once

#include <string>
#include <utility>

#include "maybe.hpp"

namespace mtl {

template <typename OkType, typename ErrorType>
class result {
  private:
    struct ok_tag {};
    struct err_tag {};
    union {
        OkType ok_;
        ErrorType err_;
    };
    enum class tag { OK, ERR, NONE } tag_;

    // Ctors with tags so they know which type to construct in-place
    template <class... Args>
    result(ok_tag, Args&&... args) : ok_(std::forward<Args>(args)...), tag_(tag::OK) {}
    template <class... Args>
    result(err_tag, Args&&... args) : err_(std::forward<Args>(args)...), tag_(tag::ERR) {}

  public:
    template <class... Args>
    static result ok(Args&&... args) {
        return result(ok_tag{}, std::forward<Args>(args)...);
    }

    template <class... Args>
    static result err(Args&&... args) {
        return result(err_tag{}, std::forward<Args>(args)...);
    }

    /// @todo This should only be defined if either the OkType or ErrorType are non-trivial
    ~result() {}

    // Observers
    bool is_ok() const { return tag_ == tag::OK; }
    bool is_err() const { return !is_ok(); }
    /// @todo Allow for default values

    const OkType& get_ok() const { return ok_; }
    const ErrorType& get_err() const { return err_; }

    // Modifiers
    OkType& get_ok() { return ok_; }
    ErrorType& get_err() { return err_; }
    // Ownership modifiers
    OkType&& release_ok() {
        tag_ = tag::NONE;
        return std::move(ok_);
    }
    ErrorType&& release_err() {
        tag_ = tag::NONE;
        return std::move(err_);
    }

    // Maybe-wrapped observers and modifiers
    /*
    maybe<const OkType&> maybe_get_ok() const {
        if (is_ok()) {
            return ok_;
        } else {
            return maybe<const OkType&>::none();
        }
    }
    maybe<const ErrorType&> maybe_get_err() const {
        if (is_err()) {
            return err_;
        } else {
            return maybe<const OkType&>::none();
        }
    }
    maybe<OkType&> maybe_get_ok() {
        if (is_ok()) {
            return ok_;
        } else {
            return maybe<OkType&>::none();
        }
    }
    maybe<ErrorType&> maybe_get_err() {
        if (is_err()) {
            return err_;
        } else {
            return maybe<ErrorType&>::none();
        }
    }
    maybe<OkType&&> maybe_release_ok() {
        if (is_ok()) {
            tag_ = tag::NONE;
            return std::move(ok_);
        } else {
            return maybe<OkType&&>::none();
        }
    }
    maybe<ErrorType&&> maybe_release_err() {
        if (is_err()) {
            tag_ = tag::NONE;
            return std::move(err_);
        } else {
            return maybe<ErrorType&&>::none();
        }
    }
    */
};

enum class error_kind {
    parse_error,
    unknown,
};

class error {
  private:
    error_kind kind_;
    std::string error_info_;

  public:
    error(error_kind kind, std::string info) : kind_(kind), error_info_(std::move(info)) {}
    error(std::string info) : kind_(error_kind::unknown), error_info_(std::move(info)) {}
    std::string stringify(error_kind kind) const {
        switch (kind) {
        case error_kind::parse_error: {
            return "parse_error";
        }
        default: {
            return "unknown";
        }
        }
    }
    std::string to_string() const {
        return "Error(" + stringify(kind_) + ") Details: " + error_info_;
    }
};
} // namespace mtl