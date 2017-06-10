#include "lib/clangdb.hh"
#include "google/protobuf/repeated_field.h"
#include "third_party/bazel/extra_actions_base.pb.h"
#include "third_party/clang/compilation_database.pb.h"
#include <fstream>
#include <iostream>

void convert(const blaze::CppCompileInfo& cpp_compile_info,
             clang::tooling::db::CompileCommand* compile_command) {
  compile_command->set_file(cpp_compile_info.source_file());
  compile_command->set_output(cpp_compile_info.output_file());

  compile_command->add_arguments(cpp_compile_info.tool());
  {
    const auto compiler_option = cpp_compile_info.compiler_option();
    std::for_each(
        compiler_option.cbegin(), compiler_option.cend(),
        [&compile_command](const std::string& option) { compile_command->add_arguments(option); });
  }
  compile_command->add_arguments("-c");
  compile_command->add_arguments(cpp_compile_info.source_file());
  compile_command->add_arguments("-o");
  compile_command->add_arguments(cpp_compile_info.output_file());
}

int main(const int argc, const char** argv) {
  if (argc != 3) {
    std::cerr << "expected three command line arguments, got " << argc << std::endl;
    return 1;
  }
  const std::string extra_action_info_pb(argv[1]);
  const std::string compile_command_pb(argv[2]);

  blaze::CppCompileInfo cpp_compile_info;
  {
    blaze::ExtraActionInfo extra_action_info;
    std::fstream f(extra_action_info_pb, f.in | f.binary);
    if (!f.is_open()) {
      std::cerr << "failed to open " << extra_action_info_pb << std::endl;
      return 1;
    }
    extra_action_info.ParseFromIstream(&f);
    cpp_compile_info = extra_action_info.GetExtension(blaze::CppCompileInfo::cpp_compile_info);
  }
  clang::tooling::db::CompileCommand compile_command;
  convert(cpp_compile_info, &compile_command);
  {
    std::fstream f(compile_command_pb, f.out | f.binary);
    if (!f.is_open()) {
      std::cerr << "failed to open " << compile_command_pb << std::endl;
      return 1;
    }
    compile_command.SerializeToOstream(&f);
  }

  return 0;
}
