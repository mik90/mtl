include(GoogleTest)

add_executable(UnitTest 
  main.cpp
  wrapped_var_test.cpp
)

target_link_libraries(UnitTest PRIVATE
  project_warnings
  project_options
  CONAN_PKG::gtest
	Mtl
)

gtest_discover_tests(UnitTest)
