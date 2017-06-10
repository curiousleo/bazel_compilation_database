action_listener(
    name = "capture_cc_command",
    mnemonics = ["CppCompile"],
    extra_actions = [":capture_cc_command_action"],
    visibility = ["//visibility:public"],
)

extra_action(
    name = "capture_cc_command_action",
    tools = ["//main:capture"],
    out_templates = ["$(ACTION_ID)_compile_command.pb"],
    cmd = "$(location //main:capture) "
        + "$(EXTRA_ACTION_FILE) "
        + "$(output $(ACTION_ID)_compile_command.pb)",
)
