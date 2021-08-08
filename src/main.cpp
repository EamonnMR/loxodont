#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>

#include <editline/readline.h>

#include "main.hpp"

const int EXIT_ERROR = 65;
const int EXIT_OK = 0;

bool hadError {false};

void report(int line, std::string where, std::string message){
  std::cout << "[line " << line << "] error where" << message << "\n";
  hadError = true;
}

void error(int line, std::string message){
  report(line, std::string{""}, message);
}


std::string repr(Literal l){
  /**
   * Get the string representation of a literal.
   */
  if(l.has_value()){
    std::string litstr;
    NonNullLiteral val {l.value()};
    std::visit([litstr](const auto &elem) mutable {
      std::stringstream str {};
      str << elem;
      litstr = str.str(); 
    }, val);
    return litstr;
  } else {
    return std::string {"null"};
  }
}

int64_t ZERO = 0;

std::string Token::toString(){
  return std::string {tokenTypeStrings[type] + ": " + lexeme + " " + repr(literal)};
}

Scanner::Scanner(std::string src){
  start = 0, current = 0, line = 0;
  std::vector<Token> tokens {};
  source = src;
  std::cout <<src;
}

std::vector<Token> Scanner::scanTokens(){
  std::cout << "ScanTokens" << "\n";
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
  return source.at(current++);
}

void Scanner::scanToken(){
  char c = {advance()};
  std::cout << "ScanToken: " << c << " Current: " << current << "\n";
  switch(c){

    case ' ':
    case '\r':
    case '\t':
      // Ignore whitespace. Fallthrough intentional
      break;

    case '\n': line++; break;

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
    case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
    case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
    case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
    case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;

    // Debugging
    // case '1': addToken(NUMBER, int64_t{1}); break;
    //case '2': addToken(NUMBER, int64_t{2}); break;
    //case '3': addToken(NUMBER, int64_t{3}); break;
    //case '4': addToken(NUMBER, int64_t{4}); break;

    case '"': scanString(); break;

    case '/': 
      if (match('/')) {
        // A comment goes until the end of the line.
        while (peek() != '\n' && !isAtEnd()) advance();
      } else {
        addToken(SLASH);
      }
      break;

    default:
      if (isDigit(c)){
        number();
      } else if (isAlpha(c)) {
        identifier();
      } else {
        error(line, "Unexpected: character"); break;
      }
  }
}

void Scanner::addToken(TokenType t){
  addToken(t, std::nullopt);
}

void Scanner::addToken(TokenType t, Literal literal){
  std::string lexeme {source.substr(start, current-start)};
  std::cout << "Add Token: Start: " << start << " Current: " << current << " Lexeme: " << lexeme << "\n";
  tokens.push_back(Token{t,source.substr(start, current-start), literal, line});
}

bool Scanner::isDigit(char c){
  return c >= '0' && c <= '9';
}

void Scanner::number(){
  while(isDigit(peek())) advance();
  if (peek() == '.' && isDigit(peekNext())){
    advance();
    while(isDigit(peek())) advance();
  }
  std::string numstr { source.substr(start, current-start)};
  addToken(NUMBER, std::stold(numstr));
}

void run(std::string source){
  Scanner scanner {source};
  std::vector<Token> tokens{scanner.scanTokens()};
  for (Token token : tokens){
    std::cout << token.toString();
    std::cout << "\n";
  }
}

bool Scanner::match(char expected){
  if(isAtEnd()) return false;
  if(source.at(current) != expected) return false;
  current ++;
  return true;
}

char Scanner::peek(){
  if(isAtEnd()) return '\0';
  return source.at(current);
}

char Scanner::peekNext(){
  if(current + 1 >= source.length()) return '\0';
  return source.at(current + 1);
}

void Scanner::scanString(){
  while(peek() != '"') {
    std::cout << "ScanString, next char: " << peek() << "\n";
    if(isAtEnd()){
      error(line, std::string{"Unterminated String"});
      break;
    }
    if (peek() == '\n') line ++;
    advance();
  }
  advance();
  std::cout << "Scanned String, start: " << start + 1 << " end: " << current - 1 << " length " << current-(start + 1) << "\n";
  // Exclude quotation marks from the string literal
  addToken(STRING, source.substr(start + 2, current - (start + 1)));
}


void Scanner::identifier(){
  //Since we called this already knowing the first 
  //char is alpha, ths rest can be numbers or
  //letters.
  while(isAlphaNumeric(peek())) advance();
  std::string text {source.substr(start, current - start)};
  TokenType type {IDENTIFIER};
  if (keywords.find(text) != keywords.end()){
    type = keywords[text];
  }
  addToken(type);
}

bool Scanner::isAlpha(char c){
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
         c == '_';
}

bool Scanner::isAlphaNumeric(char c){
  return isAlpha(c) || isDigit(c);
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
