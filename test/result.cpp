#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mtl/result.hpp"
#include "utils.hpp"

TEST(ResultTest, get_ok) {
    const auto res = mtl::Result<int, mtl::Error>::ok(5);
    ASSERT_TRUE(res.is_ok());
    ASSERT_EQ(res.get_ok(), 5);
}

TEST(ResultTest, get_err) {
    const auto res = mtl::Result<int, mtl::Error>::err("Error message");
    ASSERT_TRUE(res.is_err());
    ASSERT_EQ(res.get_err().to_string(), "Error(unknown) Details: Error message");
}

TEST(ResultTest, maybe_copy_ok) {
    auto res = mtl::Result<int, mtl::Error>::ok(5);
    auto maybe_ok = res.maybe_copy_ok();
    ASSERT_TRUE(maybe_ok.is_some());
}

TEST(ResultTest, maybe_copy_err) {
    auto res = mtl::Result<int, mtl::Error>::err("Error message");
    auto maybe_err = res.maybe_copy_err();
    ASSERT_TRUE(maybe_err.is_some());
}
TEST(ResultTest, release_ok) {
    auto res = mtl::Result<test::MoveOnlyInt, mtl::Error>::ok(5);
    auto maybe_ok = res.maybe_release_ok();
    ASSERT_TRUE(maybe_ok.is_some());
}
TEST(ResultTest, release_err) {
    auto res = mtl::Result<int, mtl::Error>::err("Error message");
    auto maybe_err = res.maybe_release_err();
    ASSERT_TRUE(maybe_err.is_some());
}