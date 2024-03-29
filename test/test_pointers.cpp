#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <type_traits>

#include "mtl/dyn_array.hpp"
#include "mtl/pointers.hpp"

TEST(PointerTest, ctor_int) {
  int* raw_ptr = new int(5);
  auto owned_ptr = mtl::OwningPtr<int>(raw_ptr);
  ASSERT_TRUE(owned_ptr.has_value());
  ASSERT_EQ(owned_ptr.get(), raw_ptr);
}

TEST(PointerTest, no_copy) {
  static_assert(!std::is_copy_constructible_v<mtl::OwningPtr<int>>);
  static_assert(!std::is_copy_assignable_v<mtl::OwningPtr<int>>);
}

TEST(PointerTest, move_constructable) {
  static_assert(std::is_move_constructible_v<mtl::OwningPtr<int>>);
  static_assert(std::is_move_assignable_v<mtl::OwningPtr<int>>);
}

TEST(PointerTest, make_owned_ptr) {
  const auto owned_ptr = mtl::make_owned<int>(1);
  ASSERT_TRUE(owned_ptr.has_value());
}

TEST(PointerTest, move_assign) {
  auto owned_ptr = mtl::make_owned<int>(1);
  auto owned_ptr_2 = std::move(owned_ptr);
  ASSERT_TRUE(owned_ptr.is_null());
  ASSERT_TRUE(owned_ptr_2.has_value());
  ASSERT_EQ(*owned_ptr_2, 1);
}

TEST(PointerTest, maybe_get_some) {
  const auto owned_ptr = mtl::make_owned<int>(1);
  auto maybe = owned_ptr.maybe_get();
  ASSERT_TRUE(maybe.is_some());
}

TEST(PointerTest, maybe_get_none) {
  const auto owned_ptr = mtl::OwningPtr<int>(nullptr);
  auto maybe = owned_ptr.maybe_get();
  ASSERT_TRUE(maybe.is_none());
}

TEST(PointerTest, ctor_array) {
  auto raw_ptr = new int[5];
  {
    raw_ptr[0] = 0;
    raw_ptr[1] = 1;
    raw_ptr[2] = 2;
    raw_ptr[3] = 3;
    raw_ptr[4] = 4;
  }
  const auto owned_ptr = mtl::OwningPtr<int[]>(raw_ptr);
  ASSERT_TRUE(owned_ptr.has_value());
  ASSERT_EQ(owned_ptr[0], 0);
  ASSERT_EQ(owned_ptr[1], 1);
  ASSERT_EQ(owned_ptr[2], 2);
  ASSERT_EQ(owned_ptr[3], 3);
  ASSERT_EQ(owned_ptr[4], 4);
}

TEST(PointerTest, move_ctor) {
  auto owned_ptr = mtl::OwningPtr<int>(new int(5));
  auto other_ptr = std::move(owned_ptr);
  ASSERT_TRUE(other_ptr.has_value());
  ASSERT_EQ(*other_ptr, 5);
  ASSERT_TRUE(owned_ptr.is_null());
}

TEST(PointerTest, overwrite) {
  // Construct one ptr
  mtl::OwningPtr<int> owned_ptr(new int(0));
  // and then overwrite it, the old 'ptr' value should be deleted since the current OwningPtr is
  // destroyed
  owned_ptr = mtl::OwningPtr<int>(new int(1));
  ASSERT_TRUE(owned_ptr.has_value());
  ASSERT_EQ(*owned_ptr, 1);
}
