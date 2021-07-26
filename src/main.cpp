#include <iostream>
#include <string>

int runFile(char* filename){
  std::ifstream input(filename);
  int lastResult = 0;
  for(std::string line; getline( input, line ); ){
    lastResult = run(line)
  }
  return lastResult;
}

int runPrompt(){
  return 0;
}

int run

int main(int argc, char** argv ){
  // TODO: Use a real argument parsing setup
  if(argc > 1){ 
    std::cout << "Usage: loxodont [script]\n";
  } else if (argc == 1){
    return runFile(argv[0]);
  } else {
    return runPrompt();
  }
  return 0;
}
