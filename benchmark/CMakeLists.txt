add_executable(MtlBenchmark
  main.cpp
  array.cpp
)

target_include_directories(MtlBenchmark PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(MtlBenchmark PRIVATE
  project_warnings
  project_options
  CONAN_PKG::benchmark
	Mtl
)
