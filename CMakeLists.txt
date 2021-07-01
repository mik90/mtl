cmake_minimum_required(VERSION 3.15)

# Based off of https://github.com/lefticus/cpp_starter_project
project(mtl CXX)
include(cmake/StandardProjectSettings.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

include(cmake/StaticAnalyzers.cmake)

option(ENABLE_TESTING "Enable Test Builds" ON)
option(ENABLE_BENCHMARKS "Enable Benchmarks" OFF)
option(BUILD_NN "Build tiny neural net lib" ON)
option(OFFLINE_CONAN "Avoids adding any conan remotes" OFF)

set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")
if(ENABLE_TESTING)
  list(APPEND CONAN_EXTRA_REQUIRES gtest/1.10.0)
endif()
if(ENABLE_BENCHMARKS)
  list(APPEND CONAN_EXTRA_REQUIRES benchmark/1.5.3)
endif()

include(cmake/RunConan.cmake)

add_subdirectory(include/mtl)

if(ENABLE_TESTING)
  find_package(GTest REQUIRED)
  enable_testing()
  message("Configuring tests...")
  add_subdirectory(test)
endif()

if(ENABLE_BENCHMARKS)
  find_package(benchmark REQUIRED)
  message("Configuring benchmarks...")
  add_subdirectory(benchmark)
endif()

if(BUILD_NN)
  message("Configuring nn..")
  add_subdirectory(nn)
endif()