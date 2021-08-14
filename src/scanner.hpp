#pragma once
#include <string>
#include <vector>
#include <array>
#include <unordered_map>

#include "token.hpp"

struct Scanner {
  std::vector<Token> tokens;
  Scanner(std::string);
  std::vector<Token> scanTokens();

  private:
  std::string source;
  size_t start;
  size_t current;
  size_t line;

  bool isAtEnd();
  char advance();
  void scanToken();
  void addToken(TokenType);
  void addToken(TokenType, LiteralVal);
  bool match(char);
  char peek();
  char peekNext();
  void scanString();
  bool isDigit(char);
  bool isAlpha(char);
  bool isAlphaNumeric(char);
  void number();
  void identifier();
};

static std::unordered_map<std::string, TokenType> keywords {
  {"and",    AND},
  {"class",  KW_CLASS},
  {"else",   ELSE},
  {"false",  FALSE},
  {"for",    KW_FOR},
  {"fun",    FUN},
  {"if",     KW_IF},
  {"nil",    NIL},
  {"or",     OR},
  {"print",  PRINT},
  {"return", RETURN},
  {"super",  SUPER},
  {"this",   THIS},
  {"true",   TRUE},
  {"var",    VAR},
  {"while",  WHILE},
};

