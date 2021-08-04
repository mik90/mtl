#pragma once

namespace mtl {
// A string literal, similar to a const std::string_view or Rust's Str
class StringLiteral {};

// An ASCII string buffer
class AsciiString {};

// A UTF-8 string buffer
class Utf8String {};
} // namespace mtl