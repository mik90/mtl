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
  test_move_only.cpp
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

  -coverage
)

target_compile_options(UnitTest PRIVATE
  --coverage
)

if(ENABLE_SANITIZERS)
  target_link_libraries(UnitTest PRIVATE
    -fsanitize=undefined,address,leak
  )
  target_compile_options(UnitTest PRIVATE
    -fsanitize=undefined,address,leak
  )
endif()

set_target_properties(UnitTest PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

gtest_discover_tests(UnitTest)