#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>

#include "mtl/wrapped_var.hpp"

// Check that it even compiles
TEST(WrappedVarTest, int_constructor) {
  mtl::WrappedVar<int> wrapped_int{3};
  auto var_with_lock = wrapped_int.get();
  ASSERT_EQ(var_with_lock.get_ref(), 3);
}

TEST(WrappedVarTest, const_ref) {
  mtl::WrappedVar<int> wrapped_int{3};
  const auto var_with_lock = wrapped_int.get();
  const auto cref = var_with_lock.get_cref();
  ASSERT_EQ(cref, 3);
}

TEST(WrappedVarTest, unique_lock) {
  mtl::WrappedVar<int> wrapped_int{3};
  const auto var_with_lock = wrapped_int.get();
  ASSERT_EQ(var_with_lock.get_cref(), 3);
}

TEST(WrappedVarTest, user_defined_type) {
  struct user_defined_struct {
    int value_0;
    float value_1;
    std::string value_3;
  };

  user_defined_struct udt{0, 0.0f, "hello"};
  mtl::WrappedVar<user_defined_struct> wrapped_udt{udt};
}

TEST(WrappedVarTest, explicit_type_deduction) {
  // Compiles
  std::string a_string{"hello"};
  mtl::WrappedVar<std::string> wrapped_string{a_string};
  /* Doesn't/Shouldn't compile:
   * std::string a_string{"hello"};
   * mtl::WrappedVar wrapped_string{a_string};
   */
}

TEST(WrappedVarTest, modify_var) {
  mtl::WrappedVar<std::string> wrapped_string{"modify me"};
  {
    auto string_accessor = wrapped_string.get();
    string_accessor.get_ref() = "you are modified";
  }

  const auto const_string_accessor = wrapped_string.get();
  ASSERT_EQ(const_string_accessor.get_cref(), "you are modified");
}

TEST(WrappedVarTest, forward_args) {
  // Similar to vector::emplace_back
  using vector_type = std::vector<int>;
  constexpr vector_type::size_type size = 5;
  constexpr vector_type::value_type value = 12;
  mtl::WrappedVar<vector_type> wrapped_string(size, value);
}

TEST(WrappedVarTest, access_from_other_thread) {
  mtl::WrappedVar<int> wrapped_int{15};
  auto owned_var = wrapped_int.get();
  std::atomic<uint32_t> try_lock_attempts{0};

  auto thread_without_lock = std::thread([&wrapped_int, &try_lock_attempts] {
    // Try to get the lock 5 times
    while (try_lock_attempts < 5) {
      ASSERT_FALSE(wrapped_int.try_lock())
          << "Mutex is already locked so it should not able to be unlocked";
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
      ++try_lock_attempts;
    }
  });

  thread_without_lock.join();
}