#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <editline/readline.h>

int run(std::string line){
  std::cout << line;
  printf("\n");
  return 0;
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

int main(int argc, char** argv ){
  // TODO: Use a real argument parsing setup
  if(argc > 2){ 
    std::cout << "Usage: loxodont [script]\n";
    return 0;
  } else if (argc == 2){
    return runFile(argv[1]);
  } else {
    return runPrompt();
  }
}
