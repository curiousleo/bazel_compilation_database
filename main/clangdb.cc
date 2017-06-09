#include <algorithm>
#include <optional>
#include "lib/clangdb.hh"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/repeated_field.h"
#include "third_party/bazel/extra_actions_base.pb.h"
#include "third_party/clang/compilation_database.pb.h"
#include <iostream>

std::optional<std::string> find_variable(std::string name, const google::protobuf::RepeatedPtrField<blaze::EnvironmentVariable>& environment_variables) {
	auto found = std::find_if(environment_variables.cbegin(), environment_variables.cend(),
		  [&name](const blaze::EnvironmentVariable& variable) {
		    return variable.name() == name;
		  });
	if (found == environment_variables.cend()) {
		return {};
	} else {
		return found->value();
	}
}

std::string exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string output;
	std::unique_ptr<FILE> pipe(popen(cmd, "r"));
	while (fgets(buffer.data(), 128, pipe.get()) != NULL) {
		output += buffer.data();
	}
	return output;
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

//  auto directory = find_variable("PWD", cpp_compile_info.variable());
//  if (!directory.has_value()) {
//	std::cerr << "cannot determine path: PWD environment variable not set" << std::endl;
//	return 1;
//  }
//  compile_command.set_directory(*directory);

  compile_command.set_file(cpp_compile_info.source_file());
  compile_command.set_output(cpp_compile_info.output_file());

  compile_command.add_arguments(cpp_compile_info.tool());
  {
	  auto compiler_option = cpp_compile_info.compiler_option();
	  std::for_each(compiler_option.cbegin(), compiler_option.cend(),
			  [&compile_command](const std::string& option) {
				compile_command.add_arguments(option);
			  });
  }
  compile_command.add_arguments("-c");
  compile_command.add_arguments(cpp_compile_info.source_file());
  compile_command.add_arguments("-o");
  compile_command.add_arguments(cpp_compile_info.output_file());

  {
	  std::unique_ptr<FILE> compile_command_file(std::fopen(argv[2], "wb"));
	  compile_command.SerializeToFileDescriptor(fileno(compile_command_file.get()));
  }
  //fclose(compile_command_file.get());
  // std::cout << compile_command.DebugString() << std::endl;
  // std::cout << exec("bazel info execution_root") << std::endl;
  return 0;
}
