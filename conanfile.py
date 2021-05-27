from conans import ConanFile, CMake, tools


class MtlConan(ConanFile):
    name = "mtl"
    description = "Config files with enforced types"
    version = "0.1"
    license = "MIT"
    url = "https://github.com/mik90/mtl"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "src/*", "CMakeLists.txt", "test/*", "cmake/*"
    no_copy_source = True

    def requirements(self):
        if tools.get_env("CONAN_RUN_TESTS"):
            self.requires("gtest/1.10.0")

    # Only for building tests
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if tools.get_env("CONAN_RUN_TESTS"):
            cmake.test()

    def package(self):
        self.copy("*.hpp", src="src", dst="mtl")
        if tools.get_env("CONAN_RUN_TESTS"):
            self.copy("*.cpp", src="test", dst="test")

    def package_id(self):
        self.info.header_only()
