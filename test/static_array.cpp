
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mtl/static_array.hpp"

TEST(StaticArrayTest, ctor_empty_array) {
    // Test that it even builds
    constexpr size_t capacity = 5;
    const auto array = mtl::StaticArray<int, capacity>();
    ASSERT_EQ(array.capacity(), capacity);
}

TEST(StaticArrayTest, ctor_init_list) {
    constexpr size_t capacity = 3;
    const auto array = mtl::StaticArray<int, capacity>({0, 1, 2});
    ASSERT_EQ(array.capacity(), capacity);
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);
    ASSERT_EQ(array[2], 2);
}

TEST(StaticArrayTest, ctor_init_list_capacity_mismatch) {
    constexpr size_t capacity = 1;
    const auto init_list = {0, 1, 2};
    ASSERT_GT(init_list.size(), capacity); // Test pre-condition

    const auto array = mtl::StaticArray<int, capacity>(init_list);
    ASSERT_EQ(array.size(), 1);
}

TEST(StaticArrayTest, ctor_empty_init_list) {
    constexpr size_t capacity = 2;

    const auto array = mtl::StaticArray<int, capacity>({});
    ASSERT_EQ(array.size(), 0);
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

TEST(StaticArrayTest, maybe_copy_at_some) {
    constexpr size_t capacity = 3;
    const auto array = mtl::StaticArray<int, capacity>({0, 1, 2});
    const auto maybe_value = array.maybe_copy_at(2);
    ASSERT_TRUE(maybe_value.is_some());
    ASSERT_EQ(maybe_value.get_some(), 2);
}

TEST(StaticArrayTest, maybe_copy_at_none) {
    constexpr size_t capacity = 3;
    const auto array = mtl::StaticArray<int, capacity>({0, 1, 2});
    const auto maybe_value = array.maybe_copy_at(5);
    ASSERT_TRUE(maybe_value.is_none());
}

TEST(StaticArrayTest, make_static_array) {
    const auto array = mtl::make_static_array<int>(0, 1, 2, 3);
    ASSERT_EQ(array.size(), 4);
    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);
    ASSERT_EQ(array[2], 2);
    ASSERT_EQ(array[3], 3);
}

/*
TEST(StaticArrayTest, remove_at_0) {
    auto array = mtl::make_static_array<int>(0, 1, 2, 3);
    const auto maybe = array.remove_at(2);
    ASSERT_TRUE(maybe);
    ASSERT_EQ(maybe.get_some(), 2);
    ASSERT_EQ(array[0], 0);
    ASSERT_EQ(array[1], 1);
    ASSERT_EQ(array[2], 3);
}
*/