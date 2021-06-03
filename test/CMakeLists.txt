include(GoogleTest)

add_executable(UnitTest 
  main.cpp
  maybe.cpp
  result.cpp
  dyn_array.cpp
  static_array.cpp
  wrapped_var.cpp
)

target_include_directories(UnitTest PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(UnitTest PRIVATE
  project_warnings
  project_options
  CONAN_PKG::gtest
	Mtl
)

gtest_discover_tests(UnitTest)
