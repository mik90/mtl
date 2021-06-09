cmake_minimum_required(VERSION 3.15)

# Based off of https://github.com/lefticus/cpp_starter_project
project(mtl CXX)
include(cmake/StandardProjectSettings.cmake)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

include(cmake/StaticAnalyzers.cmake)

option(ENABLE_TESTING "Enable Test Builds" OFF)
option(ENABLE_BENCHMARKS "Enable Benchmarks" OFF)

# Set up some extra Conan dependencies based on our needs before loading Conan
set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")
if(ENABLE_TESTING)
  list(APPEND CONAN_EXTRA_OPTIONS mtl:build_tests=True)
endif()
if(ENABLE_BENCHMARKS)
  list(APPEND CONAN_EXTRA_OPTIONS mtl:build_benchmarks=True)
endif()
set(CONAN_PROFILE default CACHE STRING "Conan profile to use")

include(cmake/Conan.cmake)
run_conan()

add_subdirectory(include/mtl)

if(ENABLE_TESTING)
  enable_testing()
  message("Configuring tests...")
  add_subdirectory(test)
endif()

if(ENABLE_BENCHMARKS)
  message("Configuring benchmarks...")
  add_subdirectory(benchmark)
endif()