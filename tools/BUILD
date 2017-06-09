action_listener(
    name = "index",
    mnemonics = ["CppCompile"],
    extra_actions = [":index_action"],
    visibility = ["//visibility:public"],
)

extra_action(
    name = "index_action",
    tools = ["//main:clangdb"],
    out_templates = ["$(ACTION_ID)_compile_command.pb"],
    cmd = "$(location //main:clangdb) "
        + "$(EXTRA_ACTION_FILE) "
        + "$(output $(ACTION_ID)_compile_command.pb)",
)
