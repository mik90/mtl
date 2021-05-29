#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "either.hpp"

TEST(EitherTest, ctor_first) {
    const auto either = mtl::either<int, float>(int{5});
    ASSERT_TRUE(either.has_first());
    ASSERT_FALSE(either.has_second());
    ASSERT_TRUE(either.has_any());
}

TEST(EitherTest, ctor_second) {
    const auto either = mtl::either<int, float>(5.0f);
    ASSERT_FALSE(either.has_first());
    ASSERT_TRUE(either.has_second());
    ASSERT_TRUE(either.has_any());
}

TEST(EitherTest, ctor_default) {
    const auto either = mtl::either<int, float>();
    ASSERT_FALSE(either.has_first());
    ASSERT_FALSE(either.has_second());
    ASSERT_FALSE(either.has_any());
}

struct error {
    error(const std::string& desc) : desc_(desc){};
    ~error() = default;
    std::string desc_;
};

TEST(EitherTest, ctor_move_only_first_type) {
    auto ptr = std::make_unique<int>(5);
    const auto either = mtl::either<std::unique_ptr<int>, error>(std::move(ptr));
    ASSERT_TRUE(either.has_first());
    ASSERT_FALSE(either.has_second());
    ASSERT_TRUE(either.has_any());
}

TEST(EitherTest, ctor_move_only_second_type) {
    auto ptr = std::make_unique<int>(5);
    const auto either = mtl::either<int, std::unique_ptr<int>>(std::move(ptr));
    ASSERT_FALSE(either.has_first());
    ASSERT_TRUE(either.has_second());
    ASSERT_TRUE(either.has_any());
}

using MoveOnlyType = std::unique_ptr<int>;
constexpr int dummy_underlying_value = 5;
MoveOnlyType dummy_move_only_value() { return std::make_unique<int>(dummy_underlying_value); }
using CopyableType = int;

TEST(EitherTest, release_move_only_first_type) {
    auto either = mtl::either<MoveOnlyType, CopyableType>(dummy_move_only_value());
    const auto owned_ptr = either.release_first();
    ASSERT_NE(owned_ptr, nullptr);
}

TEST(EitherTest, release_move_only_second_type) {
    auto either = mtl::either<CopyableType, MoveOnlyType>(dummy_move_only_value());
    const auto owned_ptr = either.release_second();
    ASSERT_NE(owned_ptr, nullptr);
}
// either() should be constructable from non-rvalue types
/*
TEST(EitherTest, release_copyable_first_type) {
    CopyableType dummy_copyable_value = 10;
    auto either = mtl::either<CopyableType, MoveOnlyType>(dummy_copyable_value);
    const auto int_value = either.release_first();
    ASSERT_NE(int_value, nullptr);
}

TEST(EitherTest, release_copyable_second_type) {
    CopyableType dummy_copyable_value = 10;
    auto either = mtl::either<MoveOnlyType, CopyableType>(dummy_copyable_value);
    const auto int_value = either.release_second();
    ASSERT_NE(int_value, nullptr);
}
*/