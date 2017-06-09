#!/usr/bin/env bash

# https://stackoverflow.com/a/21188136
abspath() {
  # $1 : relative filename
  echo "$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
}

BAZEL_TARGET=$1
INDEX_ROOT="$(bazel info execution_root)/bazel-out/local-fastbuild/extra_actions/tools/index_action"
DB_OUT=$(abspath "compile_commands.json")

# bazel build --experimental_action_listener='//tools:index' "$BAZEL_TARGET"
# bazel run //main:collect "$INDEX_ROOT" "$DB_OUT"

echo "bazel build --experimental_action_listener='//tools:index' \"$BAZEL_TARGET\""
echo "bazel run //main:collect \"$INDEX_ROOT\" \"$DB_OUT\""
