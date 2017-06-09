#include "lib/clangdb.hh"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/repeated_field.h"
#include "third_party/bazel/extra_actions_base.pb.h"
#include "third_party/clang/compilation_database.pb.h"
#include <iostream>

void convert(const blaze::CppCompileInfo& cpp_compile_info,
             clang::tooling::db::CompileCommand& compile_command) {
  compile_command.set_file(cpp_compile_info.source_file());
  compile_command.set_output(cpp_compile_info.output_file());

  compile_command.add_arguments(cpp_compile_info.tool());
  {
    auto compiler_option = cpp_compile_info.compiler_option();
    std::for_each(
        compiler_option.cbegin(), compiler_option.cend(),
        [&compile_command](const std::string& option) { compile_command.add_arguments(option); });
  }
  compile_command.add_arguments("-c");
  compile_command.add_arguments(cpp_compile_info.source_file());
  compile_command.add_arguments("-o");
  compile_command.add_arguments(cpp_compile_info.output_file());
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "expected three command line arguments, got " << argc << std::endl;
    return 1;
  }

  blaze::CppCompileInfo cpp_compile_info;
  {
    blaze::ExtraActionInfo extra_action_info;
    std::unique_ptr<FILE> extra_action_info_file(std::fopen(argv[1], "rb"));
    extra_action_info.ParseFromFileDescriptor(fileno(extra_action_info_file.get()));
    cpp_compile_info = extra_action_info.GetExtension(blaze::CppCompileInfo::cpp_compile_info);
  }
  clang::tooling::db::CompileCommand compile_command;
  convert(cpp_compile_info, compile_command);
  {
    std::unique_ptr<FILE> compile_command_file(std::fopen(argv[2], "wb"));
    compile_command.SerializeToFileDescriptor(fileno(compile_command_file.get()));
  }

  return 0;
}
