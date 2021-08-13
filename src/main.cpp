#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <editline/readline.h>

#include "token.hpp"
#include "scanner.hpp"
#include "main.hpp"

bool hadError {false};

void report(size_t line, std::string where, std::string message){
  std::cout << "[line " << line << "] error where" << message << "\n";
  hadError = true;
}

void error(size_t line, std::string message){
  report(line, std::string{""}, message);
}

void run(std::string source){
  Scanner scanner {source};
  std::vector<Token> tokens{scanner.scanTokens()};
  for (Token token : tokens){
    std::cout << token.toString();
    std::cout << "\n";
  }
}

void runFile(char* filename){
  std::string filenameStr { filename };
  std::ifstream input{filenameStr};
  std::stringstream buffer;
  buffer << input.rdbuf();
  run(buffer.str());
}

void runPrompt(){
  while(true){
    hadError = false;
    char* input_raw {readline("loxodont> ")};
    add_history(input_raw);
    std::string input {input_raw};
    std::cout << input;
    run(input);
  }
}
#ifndef testmode

const int EXIT_ERROR = 65;
const int EXIT_OK = 0;
int main(int argc, char** argv ){
  // TODO: Use a real argument parsing setup
  if(argc > 2){ 
    std::cout << "Usage: loxodont [script]\n";
    return EXIT_OK;
  } else if (argc == 2){
    runFile(argv[1]);
  } else {
    runPrompt();
  }
  return hadError ? EXIT_ERROR : EXIT_OK;
}
#endif
