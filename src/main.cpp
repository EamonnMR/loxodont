#include <iostream>

int runFile(char*){
  return 0;
}

int runPrompt(){
  return 0;
}

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
