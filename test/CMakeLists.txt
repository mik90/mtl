include(GoogleTest)

add_executable(UnitTest
  algorithm.cpp
  dyn_array.cpp
  iterator.cpp
  hash_map.cpp
  main.cpp
  maybe.cpp
  static_array.cpp
  pointers.cpp
  result.cpp
  utils.hpp
  wrapped_var.cpp
)

target_include_directories(UnitTest PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(UnitTest PRIVATE
  project_warnings
  project_options
  GTest::gtest
	mtl
)

gtest_discover_tests(UnitTest)