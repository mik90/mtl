include(GoogleTest)

add_executable(UnitTest
  main.cpp
  test_algorithm.cpp
  test_dyn_array.cpp
  test_hash_map.cpp
  test_iterator.cpp
  test_maybe.cpp
  test_static_array.cpp
  test_memoize.cpp
  test_pointers.cpp
  test_result.cpp
  test_wrapped_var.cpp
  utils.hpp
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