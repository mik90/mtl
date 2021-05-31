#pragma once

#include "either.hpp"
#include <string>
#include <utility>

namespace mtl {

template <typename OkType, typename ErrorType>
class result {
  private:
    mtl::either<OkType, ErrorType> either_;
    struct ok_tag {};
    struct err_tag {};

  public:
    template <class... Args>
    static result ok(Args&&... args) {
        return result(ok_tag{}, std::forward<Args>(args)...);
    }

    template <class... Args>
    static result err(Args&&... args) {
        return result(err_tag{}, std::forward<Args>(args)...);
    }

    template <class... Args>
    result(ok_tag, Args&&... args) {
        either_ = either::first_emplace(std::forward<Args>(args)...);
    }

    template <class... Args>
    result(err_tag, Args&&... args) {
        either_ = either::second_emplace(std::forward<Args>(args)...);
    }

    bool is_ok() const {
        if (either_.has_none()) {
            // If valueless, then it's not ok
            return false;
        } else if (either_.has_first()) {
            // An OkType is held
            return true;
        }
        // An OkType is not held
        return false;
    }
    bool is_err() const { return !this->is_ok(); }

    OkType& get_ok() { return either_.get_first(); }

    const OkType& get_ok() const { return either_.get_first(); }

    ErrorType& get_err() { return either_.get_second(); }

    const ErrorType& get_err() const { return either_.get_second(); }

    OkType&& release_ok() { return either_.release_first(); }

    ErrorType&& release_err() { return either_.release_second(); }
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
            return "Unknown error";
        }
        }
    }
    std::string to_string() const {
        return "error: " + stringify(kind_) + " Details: " + error_info_;
    }
};
} // namespace mtl