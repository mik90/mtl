#include "mtl/dyn_array.hpp"
#include "utils.hpp"

#include <cmath>
#include <numeric>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(DynArrayTest, ctor) {
    mtl::DynArray<int> arr;
    ASSERT_EQ(arr.size(), 0);
}

TEST(DynArrayTest, dtor) {
    {
        mtl::DynArray<int> arr({0, 1, 2});
        ASSERT_EQ(arr.size(), 3);
    }
    SUCCEED();
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
        ASSERT_EQ(array[i], i) << "array[" << i << "] = " << array[i] << " instead of " << i;
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
        ASSERT_EQ(array[i], i) << "array[" << i << "] = " << array[i] << " instead of " << i;
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
        ASSERT_EQ(array[i], i) << "array[" << i << "] = " << array[i] << " instead of " << i;
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

/// Capacity is decreased to below size, expect that the size is reset to the lower capacity
TEST(DynArrayTest, set_capacity_below_size) {
    using mtl::u8;

    constexpr u8 starting_capacity = 48;
    auto array = mtl::DynArray<u8>::make_with_capacity(starting_capacity);
    ASSERT_EQ(array.capacity(), starting_capacity);
    {
        for (u8 i = 0; i < starting_capacity; ++i) {
            array.push_back(i);
        }
        ASSERT_EQ(array.size(), starting_capacity);

        for (u8 i = 0; i < starting_capacity; ++i) {
            ASSERT_EQ(array[i], i) << "array[" << i << "] = " << array[i] << " instead of " << i;
        }
    }

    {
        constexpr u8 smaller_capacity = 12;

        // Decrease the capacity below the size
        array.set_capacity(smaller_capacity);
        ASSERT_EQ(array.capacity(), smaller_capacity);

        ASSERT_EQ(array.size(), smaller_capacity);

        for (u8 i = 0; i < smaller_capacity; ++i) {
            ASSERT_EQ(array[i], i) << "array[" << i << "] = " << array[i] << " instead of " << i;
        }
    }
}

TEST(DynArrayTest, make_with_capacity) {
    const auto arr = mtl::DynArray<int>::make_with_capacity(25);
    ASSERT_EQ(arr.capacity(), 25);
}

TEST(DynArrayTest, copy_const) {
    const auto arr = mtl::DynArray<int>({0, 1, 2, 3});
    auto new_arr = arr.copy();
    auto newer_arr(arr.copy());
}

TEST(DynArrayTest, overwrite_with_r_value) {
    auto arr = mtl::DynArray<int>();
    arr = mtl::DynArray<int>({0, 1, 2, 3});

    ASSERT_EQ(arr[0], 0);
    ASSERT_EQ(arr[1], 1);
    ASSERT_EQ(arr[2], 2);
    ASSERT_EQ(arr[3], 3);
}

TEST(DynArrayTest, overwrite_with_l_value) {
    const mtl::DynArray<int> l_value_arr = {0, 1, 2, 3};
    auto arr =
        mtl::DynArray<int>({9, 8, 7, 6}); // <-- arr is being leaked (value from allocate_new)
    arr = l_value_arr;

    ASSERT_EQ(arr[0], 0);
    ASSERT_EQ(arr[1], 1);
    ASSERT_EQ(arr[2], 2);
    ASSERT_EQ(arr[3], 3);
}

TEST(DynArrayTest, fill_capacity) {
    auto arr = mtl::DynArray<int>::make_with_capacity(50);
    ASSERT_EQ(arr.size(), 0);
    arr.fill_with(5);
    for (const auto& elem : arr) {
        ASSERT_EQ(elem, 5);
    }
}

TEST(DynArrayTest, fill_capacity_generator_func) {
    auto arr = mtl::DynArray<int>::make_with_capacity(50);
    ASSERT_EQ(arr.size(), 0);
    arr.fill_with_generator([] { return 5; });
    for (const auto& elem : arr) {
        ASSERT_EQ(elem, 5);
    }
}

TEST(DynArrayTest, emplace_back) {
    auto arr = mtl::DynArray<int>();
    arr.emplace_back(3);
    ASSERT_EQ(arr.size(), 1);
    ASSERT_EQ(arr[0], 3);
}

TEST(DynArrayTest, emplace_back_move_only) {
    auto arr = mtl::DynArray<test::MoveOnlyInt>();
    arr.emplace_back(3);
    ASSERT_EQ(arr.size(), 1);
    ASSERT_EQ(arr[0], 3);
}

/* Doesn't work, DynArray default constructs data
TEST(DynArrayTest, emplace_back_non_default_Ctor) {
    auto arr = mtl::DynArray<test::NonDefaultConstructableType>();
    arr.emplace_back(3);
    ASSERT_EQ(arr.size(), 1);
    ASSERT_EQ(arr[0], 3);
}
*/