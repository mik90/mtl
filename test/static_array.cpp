
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mtl/static_array.hpp"

TEST(StaticArrayTest, ctor) {
    // Test that it even builds
    constexpr size_t capacity = 5;
    const auto array = mtl::static_array<int, capacity>();
    ASSERT_EQ(array.capacity(), capacity);
}

TEST(StaticArrayTest, init_list) {
    // Test that it even builds
    constexpr size_t capacity = 3;
    const auto array = mtl::static_array<int, capacity>({0, 1, 2});
    ASSERT_EQ(array.capacity(), capacity);
}
