#include "mtl/dyn_array.hpp"
#include <cmath>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <limits>

TEST(DynArrayTest, ctor) {
    mtl::DynArray<int> arr;
    ASSERT_EQ(arr.size(), 0);
}

TEST(DynArrayTest, ctor_init_list) {
    const auto array = mtl::DynArray<int>({0, 1, 2});
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);
    ASSERT_EQ(array[2], 2);
}

TEST(DynArrayTest, push_back_ensure_size) {
    auto array = mtl::DynArray<int>();
    ASSERT_EQ(array.size(), 0);

    array.push_back(0);
    ASSERT_EQ(array.size(), 1);

    array.push_back(1);
    ASSERT_EQ(array.size(), 2);

    array.push_back(2);
    ASSERT_EQ(array.size(), 3);

    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);
    ASSERT_EQ(array[2], 2);
}

TEST(DynArrayTest, set_capacity_smaller) {
    auto array = mtl::DynArray<int>();
    array.set_capacity(2);
    ASSERT_EQ(array.capacity(), 2);
}

TEST(DynArrayTest, set_capacity_larger) {
    auto array = mtl::DynArray<std::size_t>();
    // Default capacity is 12
    const std::size_t capacity = 45;
    array.set_capacity(capacity);
    ASSERT_EQ(array.capacity(), capacity);
}

TEST(DynArrayTest, default_capacity) {
    auto array = mtl::DynArray<std::size_t>();
    // Default capacity is 12
    const std::size_t capacity = 12;
    ASSERT_EQ(array.capacity(), capacity);

    for (std::size_t i = 0; i < capacity; ++i) {
        array.push_back(i);
    }
    ASSERT_EQ(array.size(), capacity);

    // The indices should be equal to the values
    for (std::size_t i = 0; i < capacity; ++i) {
        ASSERT_EQ(array[i], i) << "Expected array[" << i << "] = " << array[i] << " instead of "
                               << i;
    }
}

TEST(DynArrayTest, resize_once) {
    auto array = mtl::DynArray<std::size_t>();
    // Default capacity is 12
    const std::size_t capacity = 13;
    array.set_capacity(capacity);
    ASSERT_EQ(array.capacity(), capacity);

    for (std::size_t i = 0; i < capacity; ++i) {
        array.push_back(i);
    }
    ASSERT_EQ(array.size(), capacity);

    // The indices should be equal to the values
    for (std::size_t i = 0; i < capacity; ++i) {
        ASSERT_EQ(array[i], i) << "Expected array[" << i << "] = " << array[i] << " instead of "
                               << i;
    }
}

TEST(DynArrayTest, many_elements) {
    auto array = mtl::DynArray<std::size_t>();
    const std::size_t n_elements = 1024;
    array.set_capacity(n_elements);
    ASSERT_EQ(array.capacity(), n_elements);

    for (std::size_t i = 0; i < n_elements; ++i) {
        array.push_back(i);
    }
    ASSERT_EQ(array.size(), n_elements);

    // The indices should be equal to the values
    for (std::size_t i = 0; i < n_elements; ++i) {
        ASSERT_EQ(array[i], i) << "Expected array[" << i << "] = " << array[i] << " instead of "
                               << i;
    }
}

TEST(DynArrayTest, ctor_double) {
    mtl::DynArray<double> arr;
    ASSERT_EQ(arr.size(), 0);
}

TEST(DynArrayTest, ctor_init_list_double) {
    const double value_0 = 0.0;
    const double value_1 = 1.2;
    const double value_2 = -2.7;
    const auto array = mtl::DynArray<double>({value_0, value_1, value_2});
    ASSERT_EQ(array.size(), 3);
    ASSERT_DOUBLE_EQ(array[0], value_0);
    ASSERT_DOUBLE_EQ(array[1], value_1);
    ASSERT_DOUBLE_EQ(array[2], value_2);
}

TEST(DynArrayTest, _set_capacity_double) {
    mtl::DynArray<double> arr;
    arr.set_capacity(25);
    ASSERT_EQ(arr.capacity(), 25);
}
