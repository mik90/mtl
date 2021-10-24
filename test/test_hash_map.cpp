#include "mtl/hash_map.hpp"
#include "utils.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mtl;

TEST(HashMapTest, default_ctor) { auto map = HashMap<std::string, size_t>(); }

TEST(HashTest, hash_string) {
    std::string value = "hello world";
    const auto hash = Hasher::JenkinsOneAtATime(value.data(), value.length());
    ASSERT_NE(hash, 0);
}