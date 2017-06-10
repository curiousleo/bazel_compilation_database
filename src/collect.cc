#include "google/protobuf/util/json_util.h"
#include "third_party/clang/compilation_database.pb.h"
#include <algorithm>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::experimental::filesystem;

bool ends_with(const std::string& s, const std::string& suffix) {
  return s.size() >= suffix.size() && std::equal(suffix.crbegin(), suffix.crend(), s.crbegin());
}

bool is_action_file(const fs::directory_entry& entry) {
  return fs::is_regular_file(entry) && ends_with(entry.path().string(), "_compile_command.pb");
}

void process_compile_command(const std::string& command_directory, std::ostream* compile_command_db,
                             const fs::path& compile_command_pb_path) {
  google::protobuf::util::JsonPrintOptions json_print_options;
  json_print_options.add_whitespace = true;

  clang::tooling::db::CompileCommand compile_command;
  {
    std::fstream f(compile_command_pb_path, f.in | f.binary);
    compile_command.ParseFromIstream(&f);
  }
  compile_command.set_directory(command_directory);

  std::string compile_command_json_out;
  google::protobuf::util::MessageToJsonString(compile_command, &compile_command_json_out,
                                              json_print_options);

  *compile_command_db << compile_command_json_out;
}

int main(const int argc, const char** argv) {
  if (argc < 4) {
    std::cerr << "expected at least four command line arguments, got " << argc << std::endl;
    return 1;
  }
  const std::string action_directory(argv[1]);
  const std::string command_directory(argv[2]);
  const std::string compile_command_db_json(argv[3]);

  std::fstream compile_command_db(compile_command_db_json, compile_command_db.out);
  fs::recursive_directory_iterator action_files(action_directory);

  compile_command_db << "[\n  ";
  for (const auto& f : action_files) {
    if (!is_action_file(f)) {
      continue;
    }
    process_compile_command(command_directory, &compile_command_db, f.path());
    compile_command_db << ",\n";
  }
  compile_command_db.seekp(-2, std::ios_base::cur);  // JSON!!!
  compile_command_db << "]";
}
