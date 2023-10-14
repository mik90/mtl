#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "mtl/result.hpp"
#include "utils.hpp"

TEST(ResultTest, get_ok) {
  const auto res = mtl::Result<int, mtl::Error>::ok(5);
  ASSERT_TRUE(res.is_ok());
  ASSERT_EQ(res.get_ok(), 5);
}

TEST(ResultTest, get_err) {
  const auto res = mtl::Result<int, mtl::Error>::err("Error message");
  ASSERT_TRUE(res.is_err());
  ASSERT_EQ(res.get_err().to_string(), "Error(unknown) Details: Error message");
}

TEST(ResultTest, maybe_copy_ok) {
  auto res = mtl::Result<int, mtl::Error>::ok(5);
  auto maybe_ok = res.maybe_copy_ok();
  ASSERT_TRUE(maybe_ok.is_some());
}

TEST(ResultTest, maybe_copy_err) {
  auto res = mtl::Result<int, mtl::Error>::err("Error message");
  auto maybe_err = res.maybe_copy_err();
  ASSERT_TRUE(maybe_err.is_some());
}
TEST(ResultTest, release_ok) {
  auto res = mtl::Result<test::MoveOnlyInt, mtl::Error>::ok(5);
  auto maybe_ok = res.maybe_release_ok();
  ASSERT_TRUE(maybe_ok.is_some());
}

TEST(ResultTest, release_err) {
  auto res = mtl::Result<int, mtl::Error>::err("Error message");
  auto maybe_err = res.maybe_release_err();
  ASSERT_TRUE(maybe_err.is_some());
}

mtl::Result<int, mtl::Error> returns_ok() { return {mtl::Ok{}, 5}; }
mtl::Result<int, mtl::Error> returns_err() { return {mtl::Err{}, mtl::Error("oopsie")}; }
mtl::Result<int, mtl::Error> returns_err_2_args() {
  return {mtl::Err{}, mtl::Error(mtl::ErrorKind::parse_error, "info")};
}

TEST(ResultTest, return_ok) {
  const auto res = returns_ok();
  ASSERT_TRUE(res.is_ok());
  ASSERT_EQ(res.get_ok(), 5);
}

TEST(ResultTest, return_err_one_arg) {
  const auto res = returns_err();
  ASSERT_TRUE(res.is_err());
  const auto err = res.get_err().to_string();
  ASSERT_TRUE(err.find("oopsie") != std::string::npos);
}

TEST(ResultTest, return_err_two_arg) {
  const auto res = returns_err_2_args();
  ASSERT_TRUE(res.is_err());
  const auto err = res.get_err().to_string();
  ASSERT_TRUE(err.find("info") != std::string::npos);
  ASSERT_TRUE(err.find("parse_error") != std::string::npos);
}