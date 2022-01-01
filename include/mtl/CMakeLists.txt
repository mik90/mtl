add_library(mtl INTERFACE)

target_link_libraries(mtl INTERFACE
  project_options
  project_warnings
)

target_include_directories(mtl INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/..
)
