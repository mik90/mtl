load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

cc_library(
    name = "utils",
    hdrs = ["utils.hpp"],
    testonly = True,
)

cc_test(
    name = "test_mtl",
    srcs = glob(include = ["*.cpp"],
    exclude = ["test_linked_list.cpp", "test_hash_map.cpp", "test_view.cpp"]),
    deps = [
        "//mtl",
        ":utils",
        "@googletest//:gtest_main",
        ],
    copts = ['-Werror']
)
