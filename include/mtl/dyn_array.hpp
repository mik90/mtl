#pragma once

#include "mtl/iterator.hpp"
#include "mtl/maybe.hpp"
#include "mtl/pointers.hpp"
#include "mtl/types.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <new>
#include <utility>

namespace mtl {
/// Array that can grow and shrink
template <typename ValueType>
class DynArray {
  public:
    DynArray();
    static DynArray make_with_capacity(usize capacity);

    DynArray(std::initializer_list<ValueType> list);

    DynArray copy() const;

    DynArray& operator=(const DynArray& other);
    DynArray& operator=(DynArray&& other);

    void set_capacity(usize new_capacity);

    template <class... Args>
    void emplace_back(Args&&... args);

    void push_back(const ValueType& value);
    void push_back(ValueType&& value);

    // Maybe remove these two?
    /// fill entirety of the array with a given value
    void fill_with(const ValueType& value);
    /// Where GeneratorFunc is a callable that returns a value
    template <typename GeneratorFunc>
    void fill_with_generator(GeneratorFunc&& func);

    ValueType& operator[](usize idx);
    const ValueType& operator[](usize idx) const noexcept;

    Maybe<ValueType> maybe_copy_at(usize idx) const;

    usize capacity() const noexcept { return capacity_; }
    usize size() const noexcept { return size_; }
    bool is_empty() const noexcept { return size_ == 0; }
    bool is_not_empty() const noexcept { return !is_empty(); }

    ValueType* data() noexcept { return data_.get(); }
    const ValueType* data() const noexcept { return data_.get(); }
    OwningPtr<ValueType[]> take_data() { return OwningPtr<ValueType[]>(data_.release()); }

    Iterator<ValueType> iter();

    ConstIterator<ValueType> c_iter() const;

    // Dumb iterators
    ValueType* begin() noexcept { return data_.get(); }
    ValueType* end() noexcept { return data_.get() + size_; }

    const ValueType* begin() const noexcept { return cbegin(); }
    const ValueType* end() const noexcept { return cend(); }

    const ValueType* cbegin() const noexcept { return data_.get(); }
    const ValueType* cend() const noexcept { return data_.get() + size_; }

  private:
    static constexpr usize default_capacity_ = 12;
    usize size_ = 0;
    usize capacity_ = default_capacity_;
    OwningPtr<ValueType[]> data_;

    // Allocates a copy of data_
    OwningPtr<ValueType[]> copy_data(const DynArray& other);

    // Allocates the memory needed for data_
    OwningPtr<ValueType[]> allocate_new();

    /// Private so that you must explicitly use `copy()` in order to copy a DynArray
    DynArray(const DynArray& other);

    /// Private so that the named ctor must be used
    DynArray(usize capacity);

    // Generically increase capacity by some set factor
    void increase_capacity();
};

// Constructors

template <typename ValueType>
DynArray<ValueType>::DynArray() : size_(0), capacity_(default_capacity_), data_(allocate_new()) {}

template <typename ValueType>
DynArray<ValueType>::DynArray(usize capacity)
    : size_(0), capacity_(capacity), data_(allocate_new()) {}

template <typename ValueType>
DynArray<ValueType>::DynArray(std::initializer_list<ValueType> list)
    : size_(0), capacity_(default_capacity_) {
    data_ = allocate_new();

    auto begin = list.begin();
    auto end = list.end();
    if (list.size() > capacity_) {
        set_capacity(list.size());
    }

    std::for_each(begin, end,
                  [this](const auto& item) { return this->data_[this->size_++] = item; });
}

// Copy ctor
template <typename ValueType>
DynArray<ValueType>::DynArray(const DynArray& other)
    : size_(other.size()), capacity_(other.capacity()), data_(copy_data(other)) {}

template <typename ValueType>
DynArray<ValueType> DynArray<ValueType>::make_with_capacity(usize capacity) {
    return DynArray(capacity);
}

// Private helpers

template <typename ValueType>
void DynArray<ValueType>::increase_capacity() {
    set_capacity(capacity_ * 2);
}

template <typename ValueType>
OwningPtr<ValueType[]> DynArray<ValueType>::copy_data(const DynArray<ValueType>& other) {
    auto copy_ptr = new ValueType[other.capacity()];
    std::memcpy(copy_ptr, other.data(), other.size() * sizeof(ValueType));
    return OwningPtr<ValueType[]>(copy_ptr);
}

template <typename ValueType>
OwningPtr<ValueType[]> DynArray<ValueType>::allocate_new() {
    auto raw_ptr = new ValueType[capacity_];
    return OwningPtr<ValueType[]>(raw_ptr);
}

// Public helpers

template <typename ValueType>
DynArray<ValueType> DynArray<ValueType>::copy() const {
    return DynArray<ValueType>(*this);
}

template <typename ValueType>
Maybe<ValueType> DynArray<ValueType>::maybe_copy_at(usize idx) const {
    if (idx > size_) {
        return None{};
    }

    return Some{data_[idx]};
}

// Modifiers
template <typename ValueType>
void DynArray<ValueType>::set_capacity(usize new_capacity) {
    if (new_capacity == capacity_) {
        // No change
        return;
    }
    if (new_capacity < size_) {
        // New capacity is smol-er
        size_ = new_capacity;
    }
    // Update capacity
    capacity_ = new_capacity;
    OwningPtr<ValueType[]> temp = std::move(data_);
    data_ = allocate_new();
    std::memmove(data_.get(), temp.get(), size_);
}

template <typename ValueType>
template <class... Args>
void DynArray<ValueType>::emplace_back(Args&&... args) {
    if (size_ >= capacity_) {
        set_capacity(capacity_ * 2);
    }
    new (&data_[size_]) ValueType(std::forward<Args>(args)...);
    ++size_;
}

template <typename ValueType>
void DynArray<ValueType>::push_back(const ValueType& value) {
    emplace_back(value);
}

template <typename ValueType>
void DynArray<ValueType>::push_back(ValueType&& value) {
    emplace_back(std::move(value));
}

// TODO Remove these and only allow standard algs

/// @brief fill entirety of the array with a given value
template <typename ValueType>
void DynArray<ValueType>::fill_with(const ValueType& value) {
    size_ = 0;
    while (size_ < capacity_) {
        data_[size_++] = value;
    }
}

/// @brief Where GeneratorFunc is a callable that returns a value
template <typename ValueType>
template <typename GeneratorFunc>
void DynArray<ValueType>::fill_with_generator(GeneratorFunc&& func) {
    size_ = 0;
    while (size_ < capacity_) {
        data_[size_++] = func();
    }
}

// Operators
template <typename ValueType>
DynArray<ValueType>& DynArray<ValueType>::operator=(const DynArray<ValueType>& other) {
    size_ = other.size();
    capacity_ = other.capacity();
    data_ = copy_data(other);
    return *this;
}

template <typename ValueType>
DynArray<ValueType>& DynArray<ValueType>::operator=(DynArray<ValueType>&& other) {
    size_ = other.size();
    capacity_ = other.capacity();
    data_ = other.take_data();
    return *this;
}

template <typename ValueType>
ValueType& DynArray<ValueType>::operator[](usize idx) {
    return data_[idx];
}

template <typename ValueType>
const ValueType& DynArray<ValueType>::operator[](usize idx) const noexcept {
    return data_[idx];
}

// Iterators
template <typename ValueType>
Iterator<ValueType> DynArray<ValueType>::iter() {
    if (size() > 0) {
        return Iterator(data());
    } else {
        return Iterator<ValueType>();
    }
}

template <typename ValueType>
ConstIterator<ValueType> DynArray<ValueType>::c_iter() const {
    if (size() > 0) {
        return ConstIterator(data());
    } else {
        return ConstIterator<ValueType>();
    }
}

} // namespace mtl