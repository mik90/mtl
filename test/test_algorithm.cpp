#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mtl/algorithm.hpp"
#include "mtl/static_array.hpp"

#include <vector>

TEST(AlgorithmTest, std_map_addition) {
    std::vector<int> vec = {-1, 0, 1, 2};
    mtl::for_each(vec.begin(), vec.end(), [](auto& i) { return ++i; });

    ASSERT_EQ(vec[0], 0);
    ASSERT_EQ(vec[1], 1);
    ASSERT_EQ(vec[2], 2);
    ASSERT_EQ(vec[3], 3);
}

TEST(AlgorithmTest, mtl_map_addition) {
    mtl::StaticArray<int, 4> arr = {-1, 0, 1, 2};
    mtl::for_each(arr.begin(), arr.end(), [](auto i) { return ++i; });

    ASSERT_EQ(arr[0], 0);
    ASSERT_EQ(arr[1], 1);
    ASSERT_EQ(arr[2], 2);
    ASSERT_EQ(arr[3], 3);
}

TEST(AlgorithmTest, mtl_map_addition_ref) {
    mtl::StaticArray<int, 4> arr = {-1, 0, 1, 2};
    mtl::for_each(arr.begin(), arr.end(), [](auto& i) { return ++i; });

    ASSERT_EQ(arr[0], 0);
    ASSERT_EQ(arr[1], 1);
    ASSERT_EQ(arr[2], 2);
    ASSERT_EQ(arr[3], 3);
}