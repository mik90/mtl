
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mtl/dyn_array.hpp"
#include "mtl/iterator.hpp"
#include <vector>

using namespace mtl;

TEST(IteratorTest, ctor) {
    const auto arr = DynArray<int>{0, 1, 2, 3};
    const auto iter = arr.c_iter();
    ASSERT_TRUE(iter.not_null());
}

TEST(IteratorTest, copy_value_some_const) {
    const auto arr = DynArray<int>{0, 1, 2, 3};
    const auto iter = arr.c_iter();
    const auto value = iter.copy_value();
    ASSERT_TRUE(value.is_some());
    ASSERT_EQ(value.unsafe_get_some(), 0);
}

TEST(IteratorTest, copy_value_some) {
    auto arr = DynArray<int>{0, 1, 2, 3};
    const auto iter = arr.iter();
    const auto value = iter.copy_value();
    ASSERT_TRUE(value.is_some());
    ASSERT_EQ(value.unsafe_get_some(), 0);
}

TEST(IteratorTest, copy_value_none) {
    const auto arr = DynArray<int>{};
    const auto iter = arr.c_iter();
    const auto value = iter.copy_value();
    ASSERT_TRUE(value.is_none());
}

TEST(IteratorTest, increment_prefix) {
    auto arr = DynArray<int>{0, 1, 2, 3};
    auto c_iter = arr.c_iter();
    auto iter = arr.iter();

    ASSERT_EQ(*++iter, arr[1]);
    ASSERT_EQ(*++c_iter, arr[1]);

    ASSERT_EQ(*++iter, arr[2]);
    ASSERT_EQ(*++c_iter, arr[2]);

    ASSERT_EQ(*++iter, arr[3]);
    ASSERT_EQ(*++c_iter, arr[3]);
}

TEST(IteratorTest, increment_postfix) {
    auto arr = DynArray<int>{0, 1, 2, 3};
    auto c_iter = arr.c_iter();
    auto iter = arr.iter();

    ASSERT_EQ(*iter++, arr[0]);
    ASSERT_EQ(*c_iter++, arr[0]);

    ASSERT_EQ(*iter++, arr[1]);
    ASSERT_EQ(*c_iter++, arr[1]);

    ASSERT_EQ(*iter++, arr[2]);
    ASSERT_EQ(*c_iter++, arr[2]);

    ASSERT_EQ(*iter++, arr[3]);
    ASSERT_EQ(*c_iter++, arr[3]);
}