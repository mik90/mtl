#include "mtl/dyn_array.hpp"
#include <cmath>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

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

TEST(DynArrayTest, set_capacity_double) {
    mtl::DynArray<double> arr;
    arr.set_capacity(25);
    ASSERT_EQ(arr.capacity(), 25);
}

TEST(DynArrayTest, set_capacity_default_to_5) {
    mtl::DynArray<double> arr;
    arr.set_capacity(5);
    ASSERT_EQ(arr.capacity(), 5);
}

/// Capacity is decreased to below size
TEST(DynArrayTest, set_capacity_below_size) {
    auto array = mtl::DynArray<std::size_t>();
    const std::size_t starting_capacity = 48;

    for (std::size_t i = 0; i < starting_capacity; ++i) {
        array.push_back(i);
    }
    ASSERT_EQ(array.size(), starting_capacity);

    const std::size_t smaller_capacity = 12;
    // The indices should be equal to the values
    array.set_capacity(smaller_capacity);

    ASSERT_EQ(array.size(), smaller_capacity);
    ASSERT_EQ(array.capacity(), smaller_capacity);

    for (std::size_t i = 0; i < smaller_capacity; ++i) {
        ASSERT_EQ(array[i], i) << "Expected array[" << i << "] = " << array[i] << " instead of "
                               << i;
    }
}

TEST(DynArrayTest, make_with_capacity) {
    const auto arr = mtl::DynArray::make_with_capacity(25);
    ASSERT_EQ(arr.capacity(), 25);
}

TEST(DynArrayTest, copy_const) {
    const auto arr = mtl::DynArray<int>({0, 1, 2, 3});
    { auto new_arr = arr.copy(); }
    { auto new_arr(arr.copy()); }
}

TEST(DynArrayTest, overwrite_with_r_value) {
    auto arr = mtl::DynArray<int>({9, 8, 7, 6});
    arr = mtl::DynArray<int>({0, 1, 2, 3});

    ASSERT_EQ(arr[0], 0);
    ASSERT_EQ(arr[1], 1);
    ASSERT_EQ(arr[2], 2);
    ASSERT_EQ(arr[3], 3);
}

TEST(DynArrayTest, overwrite_with_l_value) {
    const auto l_value_arr = mtl::DynArray<int>({0, 1, 2, 3});
    auto arr = mtl::DynArray<int>({9, 8, 7, 6});
    arr = l_value_arr;

    ASSERT_EQ(arr[0], 0);
    ASSERT_EQ(arr[1], 1);
    ASSERT_EQ(arr[2], 2);
    ASSERT_EQ(arr[3], 3);
}
