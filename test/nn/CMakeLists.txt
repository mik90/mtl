include(GoogleTest)

add_executable(nnUnitTest
  main.cpp
  test_nn.cpp
)

target_include_directories(nnUnitTest PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(nnUnitTest PRIVATE
  project_warnings
  project_options
  GTest::gtest
	nn
)

gtest_discover_tests(nnUnitTest)
