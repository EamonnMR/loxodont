#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sstream>
#include <array>
#include <variant>
#include <vector>
#include <optional>

#include <editline/readline.h>


const int EXIT_ERROR = 65;
const int EXIT_OK = 0;

typedef std::variant<bool, int64_t, long double, std::string> NonNullLiteral;
typedef std::optional<NonNullLiteral> Literal;

void cout_repr(Literal l){
  if(l.has_value()){
    NonNullLiteral val {l.value()};
    // https://stackoverflow.com/a/59477945/1048464
    // You are not expected to understand this
    std::visit([](const auto &elem) { std::cout << elem; }, val);
  } else {
    std::cout << "null";
  }
}

int64_t ZERO = 0;

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
static std::array<std::string, 39> tokenTypeStrings {
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
  size_t line;
  std::string toString();
  // TODO: Constructor
};

std::string Token::toString(){
  std::cout << tokenTypeStrings[type] << ": " << lexeme << " ";
  cout_repr(literal);
  return std::string {tokenTypeStrings[type] + ": " + lexeme + " "};
}

struct Scanner {
  std::vector<Token> tokens;
  std::string source;
  size_t start;
  size_t current;
  size_t line;

  Scanner(std::string);
  std::vector<Token> scanTokens();
  bool isAtEnd();
  char advance();
  void scanToken();
  void addToken(TokenType);
  void addToken(TokenType, Literal);
};

Scanner::Scanner(std::string src){
  std::vector<Token> tokens {};
  source = src;
  std::cout <<src;
}

std::vector<Token> Scanner::scanTokens(){
  while(!isAtEnd()){
    start = current;
    scanToken();
  }
  tokens.push_back(Token{KW_EOF, std::string{}, std::nullopt, line});
  return tokens;
}

bool Scanner::isAtEnd(){
  return current >= source.length();
}

char Scanner::advance(){
  std::cout << "current: " << current;
  return source.at(current++);
}

void Scanner::scanToken(){
  char c = {advance()};
  std::cout << c << "\n";
  switch(c){
    case '(': addToken(LEFT_PAREN); break;
    case ')': addToken(RIGHT_PAREN); break;
    case '{': addToken(LEFT_BRACE); break;
    case '}': addToken(RIGHT_BRACE); break;
    case ',': addToken(COMMA); break;
    case '.': addToken(DOT); break;
    case '-': addToken(MINUS); break;
    case '+': addToken(PLUS); break;
    case ';': addToken(SEMICOLON); break;
    case '*': addToken(STAR); break;
  }
}

void Scanner::addToken(TokenType t){
  addToken(t, std::nullopt);
}

void Scanner::addToken(TokenType t, Literal literal){
  std::string text {source.substr(start, current)};
  tokens.push_back(Token{t, text, literal, line});
}

bool hadError {false};

void run(std::string source){
  Scanner scanner {source};
  std::vector<Token> tokens{};
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
    return EXIT_OK;
  } else if (argc == 2){
    runFile(argv[1]);
  } else {
    runPrompt();
  }
  return hadError ? EXIT_ERROR : EXIT_OK;
}