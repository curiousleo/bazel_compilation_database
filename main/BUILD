cc_binary(
    name = "clangdb",
    srcs = ["clangdb.cc"],
    deps = [
        "//lib:clangdb",
	"@protobuf//:protobuf",
	"//third_party/clang:compilation_database_cc_proto",
	"//third_party/bazel:extra_actions_base_cc_proto",
    ],
    visibility = ["//visibility:public"],
)
