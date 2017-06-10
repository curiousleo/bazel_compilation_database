#!/usr/bin/env bash

find "$(bazel info output_path)/local-fastbuild/extra_actions/tools/index_action" -name '*_compile_command.pb' | \
	xargs bazel run //main:collect -- "$(bazel info execution_root)" "$(bazel info workspace)/compile_commands.json"

