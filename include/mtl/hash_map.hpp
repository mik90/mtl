#pragma once

#include <cstddef>
#include <mtl/dyn_array.hpp>
#include <mtl/static_array.hpp>

namespace mtl {

template <class HashableType>
class Hash {
  public:
  private:
    static constexpr std::size_t hash_size = 16;
    StaticArray<uint8_t, hash_size> value_;
};

template <class KeyType, class ValueType>
class HashMap {
  public:
    HashMap() {}
    /// @brief Number of key:value pairs in the map
    std::size_t count() const noexcept {
        // Keys and values are entered together so either can be used
        return keys_.size();
    }

  private:
    DynArray<KeyType> keys_;
    DynArray<ValueType> values_;
};
} // namespace mtl