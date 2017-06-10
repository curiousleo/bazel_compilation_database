#!/usr/bin/env bash

ACTION_DIRECTORY="$(bazel info output_path)/local-fastbuild/extra_actions/capture_cc_command_action"
EXEC_DIRECTORY="$(bazel info execution_root)"
COMMAND_DB_FILE="$(bazel info workspace)/compile_commands.json"

bazel run //:collect -- "$ACTION_DIRECTORY" "$EXEC_DIRECTORY" "$COMMAND_DB_FILE"
