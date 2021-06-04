
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mtl/static_array.hpp"

TEST(StaticArrayTest, ctor) {
    // Test that it even builds
    constexpr size_t capacity = 5;
    const auto array = mtl::StaticArray<int, capacity>();
    ASSERT_EQ(array.capacity(), capacity);
}

TEST(StaticArrayTest, init_list) {
    // Test that it even builds
    constexpr size_t capacity = 3;
    const auto array = mtl::StaticArray<int, capacity>({0, 1, 2});
    ASSERT_EQ(array.capacity(), capacity);
}

TEST(StaticArrayTest, push_back) {
    constexpr size_t capacity = 3;
    auto array = mtl::StaticArray<int, capacity>();
    array.push_back(0);
    array.push_back(1);
    array.push_back(2);
    ASSERT_EQ(array.size(), 3);
}