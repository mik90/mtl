
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mtl/dyn_array.hpp"
#include "mtl/iterator.hpp"
#include <vector>

using namespace mtl;

TEST(IteratorTest, iterate_over_mtl_dyn_array) {
    const auto arr = DynArray<int>{0, 1, 2, 3};
    const auto iter = Iterator(arr.get_raw_ptr());
    ASSERT_TRUE(iter.not_null());
}

// TEST(IteratorTest, iterate_over_std_vector) {}