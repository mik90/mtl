#pragma once

#include <string>
#include <utility>
#include <variant>

namespace mtl {

template <typename OkType, typename ErrorType>
class Result {
private:
  std::variant<OkType, ErrorType> variant_;
  struct ResultOkTag {};
  struct ResultErrTag {};

public:
  template <class... Args>
  static Result Ok(Args&&... args) {
    return Result(ResultOkTag{}, std::forward<Args>(args)...);
  }

  template <class... Args>
  static Result Err(Args&&... args) {
    return Result(ResultErrTag{}, std::forward<Args>(args)...);
  }

  template <class... Args>
  Result(ResultOkTag, Args&&... args)
      : variant_(std::in_place_type<OkType>, std::forward<Args>(args)...) {}

  template <class... Args>
  Result(ResultErrTag, Args&&... args)
      : variant_(std::in_place_type<ErrorType>, std::forward<Args>(args)...) {}

  bool is_ok() const {
    if (variant_.valueless_by_exception()) {
      // If valueless, then it's not Ok
      return false;
    } else if (std::holds_alternative<OkType>(variant_)) {
      // An OkType is held
      return true;
    }
    // An OkType is not held
    return false;
  }
  bool is_err() const { return !this->is_ok(); }
};

enum class ErrorKind {
  ParseError,
};

class TynError {
private:
  ErrorKind kind_;
  std::string error_info_;

public:
  TynError(ErrorKind kind, std::string info) : kind_(kind), error_info_(std::move(info)) {}
  std::string stringify(ErrorKind kind) const {
    switch (kind) {
    case ErrorKind::ParseError: {
      return "ParseError";
    }
    default: {
      return "Unknown error";
    }
    }
  }
  std::string to_string() const {
    return "Error: " + stringify(kind_) + " Details: " + error_info_;
  }
};
} // namespace mtl