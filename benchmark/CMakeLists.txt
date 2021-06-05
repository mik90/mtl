add_executable(Benchmark
  main.cpp
  array.cpp
)

target_include_directories(Benchmark PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(Benchmark PRIVATE
  project_warnings
  project_options
  CONAN_PKG::benchmark
	mtl
)
