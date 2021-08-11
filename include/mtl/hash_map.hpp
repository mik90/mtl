#pragma once

#include "mtl/maybe.hpp"
#include <cstddef>
#include <cstdint>
#include <mtl/dyn_array.hpp>
#include <mtl/static_array.hpp>
#include <mtl/types.hpp>

namespace mtl {
class Hash {
  public:
    virtual usize hash_size_bytes() const noexcept;
    // TODO How to set a "value" method of variable type here?
    virtual ~Hash();

  private:
};

/// @brief Pulled from https://en.wikipedia.org/wiki/Jenkins_hash_function
class JenkinsOneAtATime : public Hash {
  public:
    static constexpr usize hash_size = 4; // 32-bits
    usize hash_size_bytes() const noexcept override { return hash_size; };

    JenkinsOneAtATime(const u8* raw_bytes, usize size) {
        usize i = 0;
        hash_ = 0;
        while (i != size) {
            hash_ += raw_bytes[i++];
            hash_ += hash_ << 10;
            hash_ ^= hash_ >> 6;
        }
        hash_ += hash_ << 3;
        hash_ ^= hash_ >> 11;
        hash_ += hash_ << 15;
    }

    u32 value() const noexcept { return hash_; }

  private:
    u32 hash_;
};

template <class KeyType, class ValueType, class HashType = JenkinsOneAtATime>
class HashMap {
  public:
    struct KeyAndValue {
        KeyType key;
        ValueType value;
    };
    HashMap() = default;

    /// @brief Maybe return the previously existing key/pair
    mtl::Maybe<KeyAndValue> insert(KeyType key, ValueType value) {
        const auto hash = HashType(static_cast<const u8*>(&key), sizeof(KeyType));
        static_cast<void>(value);
        // TODO!
    }

    /// @brief Number of key:value pairs in the map
    usize count() const noexcept {
        // Keys and values are entered together so either can be used
        return keys_.size();
    }

  private:
    DynArray<KeyType> keys_;
    DynArray<ValueType> values_;
};
} // namespace mtl