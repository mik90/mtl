#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mtl/view.hpp"
#include "utils.hpp"

TEST(ViewTest, build_and_assign) {
  int my_int = 2;

  mtl::View my_int_view{my_int};

  {
    int my_other_int = 4;
    my_int_view = my_other_int;
    EXPECT_EQ(my_int, 4);
  }
  {
    my_int_view = 10;
    EXPECT_EQ(my_int, 10);
  }
}

TEST(ViewTest, build_const) {
  const int my_int = 2;

  mtl::View my_int_view{my_int};

  // Does not comiple
  // my_int_view.get() = 2;
  EXPECT_EQ(my_int, 2);
}
