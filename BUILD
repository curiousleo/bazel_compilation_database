action_listener(
    name = "capture",
    mnemonics = ["CppCompile"],
    extra_actions = [":capture_action"],
    visibility = ["//visibility:public"],
)

extra_action(
    name = "capture_action",
    tools = [":capture_binary"],
    out_templates = ["$(ACTION_ID)_compile_command.pb"],
    cmd = "$(location :capture_binary) "
        + "$(EXTRA_ACTION_FILE) "
        + "$(output $(ACTION_ID)_compile_command.pb)",
)

cc_binary(
    name = "capture_binary",
    srcs = ["main/capture.cc"],
    deps = [
	"@protobuf//:protobuf",
	"//third_party/bazel:extra_actions_base_cc_proto",
	"//third_party/clang:compilation_database_cc_proto",
    ],
)

cc_binary(
    name = "collect",
    srcs = ["main/collect.cc"],
    deps = [
	"@protobuf//:protobuf",
	"//third_party/clang:compilation_database_cc_proto",
    ],
    linkopts = ["-lstdc++fs"],
    visibility = ["//visibility:public"],
)
