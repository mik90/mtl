#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "result.hpp"

TEST(ResultTest, get_ok) {
    const auto res = mtl::result<int, mtl::error>::ok(5);
    ASSERT_TRUE(res.is_ok());
    ASSERT_EQ(res.get_ok(), 5);
}

TEST(ResultTest, get_err) {
    mtl::error err("Error");
    const auto res = mtl::result<int, mtl::error>::ok(err);
    ASSERT_TRUE(res.is_err());
    ASSERT_EQ(res.get_err().to_string(), "Error");
}