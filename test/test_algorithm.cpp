#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

#include "mtl/algorithm.hpp"
#include "mtl/static_array.hpp"

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

TEST(AlgorithmTest, for_each_container) {
  mtl::StaticArray<int, 4> arr = {-1, 0, 1, 2};
  mtl::for_each(arr, [](auto& i) { return ++i; });

  ASSERT_EQ(arr[0], 0);
  ASSERT_EQ(arr[1], 1);
  ASSERT_EQ(arr[2], 2);
  ASSERT_EQ(arr[3], 3);
}

TEST(AlgorithmTest, map_container) {
  mtl::StaticArray<int, 4> arr = {-1, 0, 1, 2};
  const auto new_arr = mtl::map(arr, [](const auto& i) { return i + 1; });

  ASSERT_EQ(new_arr[0], 0);
  ASSERT_EQ(new_arr[1], 1);
  ASSERT_EQ(new_arr[2], 2);
  ASSERT_EQ(new_arr[3], 3);
}
