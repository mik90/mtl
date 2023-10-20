#include <gmock/gmock.h>

#include <string>

#include "mtl/view.hpp"

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

TEST(ViewTest, compare_views) {
  int a = 2;

  mtl::View a_view{a};

  int b = 2;

  mtl::View b_view{b};

  bool eq = a_view == b_view;
  EXPECT_TRUE(eq);
  bool gt = a_view > b_view;
  EXPECT_FALSE(gt);
}

TEST(ViewTest, compare_underlying) {
  int a = 2;

  mtl::View a_view{a};

  int b = 2;

  bool res = a == b;
  EXPECT_TRUE(res);

  a_view = 3;
  EXPECT_EQ(a_view, 3);
  EXPECT_EQ(a, 3);

  a = 4;
  EXPECT_EQ(a_view, 4);
  EXPECT_EQ(4, a_view);
  EXPECT_EQ(a, 4);
  EXPECT_EQ(4, a);
}

TEST(ViewTest, operator_arrow_overload) {
  std::string word{"hello"};
  {
    mtl::View<const std::string> const_view{word};
    EXPECT_EQ(const_view->size(), 5);
  }
  mtl::View view{word};

  view->clear();
  EXPECT_TRUE(word.empty());

  EXPECT_TRUE(view->empty());
}

TEST(ViewTest, build_and_copy) {
  int my_int = 2;

  mtl::View view_a{my_int};
  mtl::View view_b{view_a};
  EXPECT_EQ(view_a, view_b);
}
