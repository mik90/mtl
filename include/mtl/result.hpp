#pragma once

#include <string>
#include <utility>

#include "maybe.hpp"

namespace mtl {

template <typename OkType, typename ErrorType>
class Result {
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
    Result(ok_tag, Args&&... args) : ok_(std::forward<Args>(args)...), tag_(tag::OK) {}
    template <class... Args>
    Result(err_tag, Args&&... args) : err_(std::forward<Args>(args)...), tag_(tag::ERR) {}

  public:
    template <class... Args>
    static Result ok(Args&&... args) {
        return Result(ok_tag{}, std::forward<Args>(args)...);
    }

    template <class... Args>
    static Result err(Args&&... args) {
        return Result(err_tag{}, std::forward<Args>(args)...);
    }

    /// @todo This should only be defined if either the OkType or ErrorType are non-trivial
    ~Result() {}

    // Observers
    bool is_ok() const { return tag_ == tag::OK; }
    bool is_err() const { return !is_ok(); }

    /// @todo Allow for default values if the err state is present
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

    // mtl::Maybe-wrapped observers and modifiers
    Maybe<OkType> maybe_copy_ok() const {
        if (is_ok()) {
            return ok_;
        } else {
            return Maybe<OkType>::none();
        }
    }
    Maybe<ErrorType> maybe_copy_err() const {
        if (is_err()) {
            return err_;
        } else {
            return Maybe<ErrorType>::none();
        }
    }
    Maybe<OkType> maybe_release_ok() {
        if (is_ok()) {
            tag_ = tag::NONE;
            return std::move(ok_);
        } else {
            return Maybe<OkType>::none();
        }
    }
    Maybe<ErrorType> maybe_release_err() {
        if (is_err()) {
            tag_ = tag::NONE;
            return std::move(err_);
        } else {
            return Maybe<ErrorType>::none();
        }
    }
};

enum class ErrorKind {
    parse_error,
    unknown,
};

class Error {
  private:
    ErrorKind kind_;
    std::string error_info_;

  public:
    Error(ErrorKind kind, std::string info) : kind_(kind), error_info_(std::move(info)) {}
    Error(std::string info) : kind_(ErrorKind::unknown), error_info_(std::move(info)) {}
    std::string stringify(ErrorKind kind) const {
        switch (kind) {
        case ErrorKind::parse_error: {
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