#pragma once

#include <string>
#include <utility>
#include <variant>

namespace mtl {

template <typename OkType, typename ErrorType>
class result {
private:
  std::variant<OkType, ErrorType> variant_;
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
  result(ok_tag, Args&&... args)
      : variant_(std::in_place_type<OkType>, std::forward<Args>(args)...) {}

  template <class... Args>
  result(err_tag, Args&&... args)
      : variant_(std::in_place_type<ErrorType>, std::forward<Args>(args)...) {}

  bool is_ok() const {
    if (variant_.valueless_by_exception()) {
      // If valueless, then it's not ok
      return false;
    } else if (std::holds_alternative<OkType>(variant_)) {
      // An OkType is held
      return true;
    }
    // An OkType is not held
    return false;
  }
  bool is_err() const { return !this->is_ok(); }

  /// @brief Throws on invalid access
  OkType& get_ok_ref() {
    return std::get<OkType>(variant_);
  }

  /// @brief Throws on invalid access
  const OkType& get_ok_ref() const {
    return std::get<OkType>(variant_);
  }
  
  /// @brief Throws on invalid access
  ErrorType& get_err_ref() {
    return std::get<ErrorType>(variant_);
  }
  
  /// @brief Throws on invalid access
  const ErrorType& get_err_ref() const {
    return std::get<ErrorType>(variant_);
  }
  
  /**
   * @todo How to do this? std::remove or some way to release the item from the variant?
  /// @brief Throws on invalid access
  OkType&& release_ok() const {
    return std::get<ErrorType>(variant_);
  }
  */
};

enum class error_kind {
  parse_error,
};

class error {
private:
  error_kind kind_;
  std::string error_info_;

public:
  error(error_kind kind, std::string info) : kind_(kind), error_info_(std::move(info)) {}
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