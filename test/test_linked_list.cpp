#include "mtl/linked_list.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mtl/types.hpp>

using namespace mtl;

TEST(LinkedListTest, default_ctor) {
    const auto list = LinkedList<int>();
    ASSERT_EQ(list.size(), 0);
}

TEST(LinkedListTest, add_elements) {
    auto list = LinkedList<int>();
    list.push_back(0);
    list.push_back(1);
    list.push_back(2);
    ASSERT_EQ(list.size(), 3);
}