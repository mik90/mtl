#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "mtl/move_only.hpp"

namespace {
  struct MyStruct {
    double a_;
    int b_;
  };
}  // namespace

// Check that it even compiles
TEST(MoveOnlyTest, ctor) {
  {
    mtl::MoveOnly myInt{5};
    mtl::MoveOnly<MyStruct> myStruct{0.5, 2};
    mtl::MoveOnly<std::string> myString{"helloWorld"};
    mtl::MoveOnly<std::string> myOtherString = "helloWorld2";
  }
  {
    std::string myMoveableType = "me";
    mtl::MoveOnly myString{std::move(myMoveableType)};
  }
}

TEST(MoveOnlyTest, access) {
  mtl::MoveOnly<std::string> myString{"hello"};
  ASSERT_EQ(*myString, "hello");
  ASSERT_EQ(myString->front(), 'h');
}

TEST(MoveOnlyTest, moveString) {
  mtl::MoveOnly<std::string> myString{"hello"};
  const auto myOtherString = std::move(myString);
  ASSERT_EQ(*myOtherString, "hello");
}

TEST(MoveOnlyTest, wrapUniquePtr) {
  mtl::MoveOnly<std::unique_ptr<std::string>> myUniqueString{
      std::make_unique<std::string>("hello")};
  auto myOtherUniqueString = std::move(myUniqueString);
  ASSERT_EQ(*(myOtherUniqueString->get()), "hello");
}

TEST(MoveOnlyTest, release) {
  mtl::MoveOnly<std::unique_ptr<std::string>> myUniqueString{
      std::make_unique<std::string>("hello")};
  auto uniquePtr = myUniqueString.release();
  ASSERT_EQ(*uniquePtr, "hello");
}

TEST(MoveOnlyTest, swap) {
  mtl::MoveOnly<std::string> first{"foo"};
  mtl::MoveOnly<std::string> second{"bar"};
  ASSERT_EQ(*first, "foo");
  ASSERT_EQ(*second, "bar");
  std::swap(first, second);
  ASSERT_EQ(*first, "bar");
  ASSERT_EQ(*second, "foo");
}