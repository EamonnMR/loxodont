#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <variant>

#include <editline/readline.h>
#include <vector>

const int EXIT_ERROR = 65;
const int EXIT_OK = 0;

// Linter error here, it seems
typedef std::variant<int64_t, long double, bool, std::string> Literal;

enum TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, KW_CLASS, ELSE, FALSE, FUN, KW_FOR, KW_IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  KW_EOF
};
// https://stackoverflow.com/a/6281535/1048464
static std::array<std::string, 39> tokenTypeStrings = {
  // Single-character tokens.
  "LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",
  "COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR",

  // One or two character tokens.
  "BANG", "BANG_EQUAL",
  "EQUAL", "EQUAL_EQUAL",
  "GREATER", "GREATER_EQUAL",
  "LESS", "LESS_EQUAL",

  // Literals.
  "IDENTIFIER", "STRING", "NUMBER",

  // Keywords.
  "AND", "KW_CLASS", "ELSE", "FALSE", "FUN", "KW_FOR", "KW_IF", "NIL", "OR",
  "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE",

  "KW_EOF"
};
struct Token {
  TokenType type;
  std::string lexeme;
  Literal literal;
  int line;
  std::string toString();
};

std::string Token::toString(){
  return std::string {tokenTypeStrings[this->type] + ": " + this->lexeme};
}

struct Lexer {
  std::vector<Token> tokens;
  std::string source;
};

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
    hadError = false;
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
