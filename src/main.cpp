#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <editline/readline.h>

const int EXIT_ERROR = 65;
const int EXIT_OK = 0;

bool hadError = false;

int run(std::string line){
  std::cout << line;
  printf("\n");
  return EXIT_OK;
}

int runFile(char* filename){
  std::string filenameStr { filename };
  std::ifstream input{filenameStr};
  std::stringstream buffer;
  buffer << input.rdbuf();
  return run(buffer.str());
}

int runPrompt(){
  while(true){
    char* input_raw = readline("loxodont> ");
    add_history(input_raw);
    std::string input {input_raw};
    run(input);
  }
  return 0;
}

void report(int line, std::string where, std::string message){
  std::cout << "[line " << line << "] error where" << message << "\n";
  hadError = true;
}

void error(int line, std::string message){
  report(line, std::string{""}, message);
}

int main(int argc, char** argv ){
  // TODO: Use a real argument parsing setup
  if(argc > 2){ 
    std::cout << "Usage: loxodont [script]\n";
    return 0;
  } else if (argc == 2){
    runFile(argv[1]);
  } else {
    runPrompt();
  }
  return hadError ? EXIT_ERROR : EXIT_OK;
}
