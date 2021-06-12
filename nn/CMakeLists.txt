add_library(nn STATIC
  nn.cpp
  nn.hpp
)

target_include_directories(nn PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(nn PUBLIC
  project_warnings
  project_options
	mtl
)