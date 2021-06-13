#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mtl/dyn_array.hpp"

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

TEST(DynArrayTest, set_capacity) {
    auto array = mtl::DynArray<int>();
    array.set_capacity(2);
    ASSERT_EQ(array.capacity(), 2);
}

TEST(DynArrayTest, many_elements) {
    auto array = mtl::DynArray<std::size_t>();
    for (std::size_t i = 0; i < 1024; ++i) {
        array.push_back(i);
    }
    // The indices should be equal to the values`
    for (std::size_t i = 0; i < 1024; ++i) {
        ASSERT_EQ(array[i], i) << "Expected array[" << i << "] was " << array[i];
    }
}