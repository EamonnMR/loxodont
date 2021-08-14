#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>

#include <editline/readline.h>

#include "token.hpp"
#include "scanner.hpp"
#include "main.hpp"

Scanner::Scanner(std::string src){
  start = 0, current = 0, line = 0;
  std::vector<Token> tokens {};
  source = src;
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
  // std::cout << "ScanToken: " << c << " Current: " << current << "\n";
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

void Scanner::addToken(TokenType t, LiteralVal literal){
  std::string lexeme {source.substr(start, current-start)};
  // std::cout << "Add Token: Start: " << start << " Current: " << current << " Lexeme: " << lexeme << "\n";
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
    // std::cout << "ScanString, next char: " << peek() << "\n";
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

