#!/usr/bin/env bash

# https://stackoverflow.com/a/21188136
abspath() {
    echo "$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
}

EXEC_ROOT=$(bazel info execution_root)
INDEX_ROOT="$EXEC_ROOT/bazel-out/local-fastbuild/extra_actions/tools/index_action"

find "$INDEX_ROOT" -name '*_compile_command.pb' | \
    xargs bazel run //main:collect -- "$EXEC_ROOT" "$(abspath compile_commands.json)"

