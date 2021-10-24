include(GoogleTest)

add_executable(UnitTest
<<<<<<< HEAD
  algorithm.cpp
  dyn_array.cpp
  iterator.cpp
  hash_map.cpp
=======
  test_algorithm.cpp
  test_dyn_array.cpp
  test_iterator.cpp
>>>>>>> 25a8625 (rename files as test)
  main.cpp
  test_maybe.cpp
  test_static_array.cpp
  test_pointers.cpp
  test_result.cpp
  utils.hpp
  test_wrapped_var.cpp
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