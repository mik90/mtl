add_library(Mtl INTERFACE)

target_link_libraries(Mtl INTERFACE
  project_options
  project_warnings
)

target_include_directories(Mtl INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/..
)
