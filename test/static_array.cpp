
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

TEST(StaticArrayTest, push_back_ensure_size) {
    constexpr size_t capacity = 3;
    auto array = mtl::StaticArray<int, capacity>();
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

TEST(StaticArrayTest, push_back_max_size) {
    constexpr size_t capacity = 1;
    auto array = mtl::StaticArray<int, capacity>();

    array.push_back(0);
    ASSERT_EQ(array.size(), 1);

    array.push_back(1);
    ASSERT_EQ(array.size(), capacity);

    array.push_back(2);
    ASSERT_EQ(array.size(), capacity);
}

/*
TEST(StaticArrayTest, maybe_copy_at) {
    constexpr size_t capacity = 3;
    const auto array = mtl::StaticArray<int, capacity>({0, 1, 2});
    const auto maybe_value = array.maybe_copy_at(2); /// @todo compiler warning here
    ASSERT_TRUE(maybe_value.is_some());
    ASSERT_EQ(maybe_value.get_some(), 2);
}
*/