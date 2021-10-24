#pragma once

#include "mtl/maybe.hpp"
#include <cstddef>
#include <cstdint>
#include <mtl/dyn_array.hpp>
#include <mtl/linked_list.hpp>
#include <mtl/types.hpp>

namespace mtl {
using Hash = u32;

/// @brief Pulled from https://en.wikipedia.org/wiki/Jenkins_hash_function
class Hasher {
  public:
    // Creates hash values from raw data
    static Hash JenkinsOneAtATime(const u8* raw_bytes, usize size) {
        usize i = 0;
        Hash hash = 0;
        while (i != size) {
            hash += raw_bytes[i++];
            hash += hash << 10;
            hash ^= hash >> 6;
        }
        hash += hash << 3;
        hash ^= hash >> 11;
        hash += hash << 15;
        return hash;
    }
    static Hash JenkinsOneAtATime(const char* raw_bytes, usize size) {
        usize i = 0;
        Hash hash = 0;
        while (i != size) {
            hash += static_cast<u32>(raw_bytes[i++]);
            hash += hash << 10;
            hash ^= hash >> 6;
        }
        hash += hash << 3;
        hash ^= hash >> 11;
        hash += hash << 15;
        return hash;
    }
};

template <class KeyType, class ValueType>
class HashMap {
  public:
    struct KeyAndValue {
        KeyType key;
        ValueType value;
    };
    HashMap() = default;

    /// @brief Maybe return the previously existing key/pair
    mtl::Maybe<KeyAndValue> insert(KeyType key, ValueType value) {
        const auto key_ptr = &key;
        const auto hash =
            Hasher::JenkinsOneAtATime(static_cast<const u8*>(key_ptr), sizeof(KeyType));
        const usize idx = hash % values_.length();
        keys_.push_back(std::move(key));
        /// @todo add to map values_[idx].emplace_back
        static_cast<void>(value);
        static_cast<void>(idx);
    }

    /// @brief Number of key:value pairs in the map
    usize count() const noexcept {
        // Keys and values are entered together so either can be used
        return keys_.size();
    }

  private:
    DynArray<KeyType> keys_;
    DynArray<LinkedList<ValueType>> values_;
};
} // namespace mtl