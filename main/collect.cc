#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/util/json_util.h"
#include "third_party/clang/compilation_database.pb.h"
#include <fstream>
#include <iostream>

int main(int argc, const char** argv) {
  if (argc != 3) {
    std::cerr << "expected three command line arguments, got " << argc << std::endl;
    return 1;
  }
  std::string command_directory(argv[1]);
  std::string compile_command_db_json(argv[2]);

  std::fstream compile_command_db(compile_command_db_json, compile_command_db.out);

  google::protobuf::util::JsonPrintOptions json_print_options;
  json_print_options.add_whitespace = true;

  clang::tooling::db::CompileCommand compile_command;
  std::string compile_command_pb;

  bool first_entry = true;  // JSON makes me sad
  compile_command_db << "[\n";
  while (std::getline(std::cin, compile_command_pb)) {
    std::fstream f(compile_command_pb, f.in | f.binary);
    compile_command.ParseFromIstream(&f);
    compile_command.set_directory(command_directory);

    std::string compile_command_json_out;
    google::protobuf::util::MessageToJsonString(compile_command, &compile_command_json_out,
                                                json_print_options);

    if (!first_entry) {
      compile_command_db << ",";
    }
    compile_command_db << compile_command_db_json;
    first_entry = false;
  }
  compile_command_db << "]\n";
}
