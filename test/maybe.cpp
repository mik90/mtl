#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mtl/maybe.hpp"
#include "utils.hpp"

TEST(MaybeTest, is_some) {
    const auto value = mtl::Maybe<int>(5);
    ASSERT_TRUE(value.is_some());
}

TEST(MaybeTest, is_none) {
    const auto value = mtl::Maybe<int>::none();
    ASSERT_TRUE(value.is_none());
}

TEST(MaybeTest, get_some_const_simple) {
    const auto value = mtl::Maybe<int>(20);
    ASSERT_EQ(value.get_some(), 20);
}

TEST(MaybeTest, get_some_non_const_simple) {
    auto value = mtl::Maybe<int>(20);
    ASSERT_EQ(value.get_some(), 20);
}

TEST(MaybeTest, release_move_only) {
    auto value = mtl::Maybe<test::MoveOnlyInt>(5);
    ASSERT_EQ(value.release(), 5);
}

TEST(MaybeTest, release_move_only_default) {
    auto value = mtl::Maybe<test::MoveOnlyInt>::none();
    ASSERT_EQ(value.release_or(5), 5);
}
/*
Don't allow this yet
TEST(MaybeTest, store_reference) {
    auto value = mtl::Maybe<const int&>(5);
    ASSERT_TRUE(value.is_some());
}
*/