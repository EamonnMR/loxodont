#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int run(std::string line){
  std::cout << line;
  printf("\n");
  return 0;
}

int runFile(char* filename){
  std::string filenameStr(filename);
  std::ifstream input(filenameStr);
  std::stringstream buffer;
  buffer << input.rdbuf();

  return run(buffer.str());
}

int runPrompt(){
  return 0;
}


int main(int argc, char** argv ){
  // TODO: Use a real argument parsing setup
  if(argc > 2){ 
    std::cout << "Usage: loxodont [script]\n";
  } else if (argc == 2){
    return runFile(argv[1]);
  } else {
    //return runPrompt();
  }
  return 0;
}
