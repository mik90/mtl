add_library(mtl INTERFACE
  algorithm.hpp
  dyn_array.hpp
  iterator.hpp
  hash_map.hpp
  maybe.hpp
  memoize.hpp
  pointers.hpp
  result.hpp
  static_array.hpp
  "string.hpp"
  wrapped_var.hpp
)

target_link_libraries(mtl INTERFACE
  project_options
  project_warnings
)

target_include_directories(mtl INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/..
)
