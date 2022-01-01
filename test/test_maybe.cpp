#include "mtl/maybe.hpp"
#include "utils.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <utility>

TEST(MaybeTest, is_some) {
    const auto value = mtl::Maybe<int>(5);
    ASSERT_TRUE(value.is_some());
}

TEST(MaybeTest, is_some_explicit) {
    const auto value = mtl::Maybe(mtl::Some<int>{5});
    ASSERT_TRUE(value.is_some());
}

TEST(MaybeTest, is_none) {
    const auto value = mtl::Maybe<int>::none();
    ASSERT_TRUE(value.is_none());
}

TEST(MaybeTest, get_some_const_simple) {
    const auto value = mtl::Maybe<int>(20);
    ASSERT_EQ(value.unsafe_get_some(), 20);
}

TEST(MaybeTest, get_some_non_const_simple) {
    auto value = mtl::Maybe<int>(20);
    ASSERT_EQ(value.unsafe_get_some(), 20);
}

TEST(MaybeTest, release_move_only) {
    auto value = mtl::Maybe<test::MoveOnlyInt>(5);
    ASSERT_EQ(value.unsafe_release(), 5);
}

TEST(MaybeTest, release_move_only_default) {
    auto value = mtl::Maybe<test::MoveOnlyInt>::none();
    ASSERT_EQ(value.release_or(5), 5);
}

mtl::Maybe<int> func_returning_some() { return 5; }
mtl::Maybe<int> func_returning_none() { return mtl::None(); }

TEST(MaybeTest, func_returns_some) {
    const auto some = func_returning_some();
    ASSERT_TRUE(some.is_some());
}

TEST(MaybeTest, func_returns_none) {
    const auto some = func_returning_none();
    ASSERT_TRUE(some.is_none());
}

mtl::Maybe<std::string> func_returning_some_string() { return "hello"; }
TEST(MaybeTest, func_returns_some_string) {
    const auto some = func_returning_some_string();
    ASSERT_TRUE(some.is_some());
}

TEST(MaybeTest, ctor_pair) {
    const auto some = mtl::Maybe<std::pair<int, double>>::some(5, 10.2);
    ASSERT_TRUE(some.is_some());
}

/*
Don't allow this yet
TEST(MaybeTest, store_reference) {
    auto value = mtl::Maybe<const int&>(5);
    ASSERT_TRUE(value.is_some());
}
*/