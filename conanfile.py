from conans import ConanFile, CMake, tools


class MtlConan(ConanFile):
    name = "mtl"
    description = "Config files with enforced types"
    version = "0.1"
    license = "MIT"
    url = "https://github.com/mik90/mtl"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "build_benchmarks": [True, False],
        "build_tests": [True, False],
    }
    default_options = {
        "build_benchmarks": False,
        "build_tests": False,
    }
    generators = "cmake"
    exports_sources = "include/*", "test/*", "CMakeLists.txt", "cmake/*"
    no_copy_source = True
    generators = "cmake"

    def requirements(self):
        if self.options.build_tests:
            self.requires("gtest/1.10.0")
        if self.options.build_benchmarks:
            self.requires("benchmark/1.5.3")

    # Only for building tests
    def build(self):
        cmake = CMake(self)
        cmake_defs = {}
        if self.options.build_tests:
            cmake_defs["ENABLE_TESTING"] = "ON"
        if self.options.build_benchmarks:
            cmake_defs["ENABLE_BENCHMARKS"] = "ON"
        cmake.configure(defs=cmake_defs)
        cmake.build()
        if self.options.build_tests:
            cmake.test(target="UnitTest")

    def package(self):
        self.copy("*.hpp", src="include/mtl", dst="mtl")

    def package_id(self):
        self.info.header_only()
