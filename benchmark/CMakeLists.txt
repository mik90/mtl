add_executable(ArrayBenchmark
  main.cpp
  array.cpp
)

target_include_directories(ArrayBenchmark PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(ArrayBenchmark PRIVATE
  project_warnings
  project_options
  benchmark::benchmark
	mtl
)
# ----------------------------------
add_executable(AlgBenchmark
  main.cpp
  array.cpp
)

target_include_directories(AlgBenchmark PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(AlgBenchmark PRIVATE
  project_warnings
  project_options
  benchmark::benchmark
	mtl
)
