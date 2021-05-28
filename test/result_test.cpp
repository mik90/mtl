#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "result.hpp"

TEST(ResultTest, ctor) {
    const auto res = mtl::result<int, mtl::error>::ok(5);
    ASSERT_TRUE(res.is_ok());
}