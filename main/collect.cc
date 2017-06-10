#include "google/protobuf/util/json_util.h"
#include "third_party/clang/compilation_database.pb.h"
#include <algorithm>
#include <fstream>
#include <iostream>

void process_compile_command(const std::string& command_directory, std::ostream* compile_command_db,
                             const std::string& compile_command_pb) {
  google::protobuf::util::JsonPrintOptions json_print_options;
  json_print_options.add_whitespace = true;

  clang::tooling::db::CompileCommand compile_command;
  {
    std::fstream f(compile_command_pb, f.in | f.binary);
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
  const std::string command_directory(argv[1]);
  const std::string compile_command_db_json(argv[2]);
  const std::string first_compile_command_pb(argv[3]);
  const std::vector<std::string> compile_command_pbs(argv + 4, argv + argc);

  std::fstream compile_command_db(compile_command_db_json, compile_command_db.out);

  compile_command_db << "[\n";
  process_compile_command(command_directory, &compile_command_db, first_compile_command_pb);
  std::for_each(compile_command_pbs.cbegin(), compile_command_pbs.cend(),
                [&command_directory, &compile_command_db](const std::string& compile_command_pb) {
                  compile_command_db << ",\n";
                  process_compile_command(command_directory, &compile_command_db,
                                          compile_command_pb);
                });
  compile_command_db << "]";
}
