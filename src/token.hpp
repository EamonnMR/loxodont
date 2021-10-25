#pragma once

#include <string>
#include <array>
#include <variant>
#include <optional>

struct NullVal : std::monostate {
};

std::ostream& operator<<(std::ostream&, const NullVal&);

typedef std::variant<NullVal, bool, int64_t, long double, std::string> LiteralVal;


std::string repr(LiteralVal);

enum TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // LiteralVals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, KW_CLASS, ELSE, FALSE, FUN, KW_FOR, KW_IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
  // ExLox Keywords
  LOCALS,

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

  // LiteralVals.
  "IDENTIFIER", "STRING", "NUMBER",

  // Keywords.
  "AND", "KW_CLASS", "ELSE", "FALSE", "FUN", "KW_FOR", "KW_IF", "NIL", "OR",
  "PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE",

  "KW_EOF"
};
struct Token {
  TokenType type;
  std::string lexeme;
  LiteralVal literal;
  size_t line;
  std::string toString();
  // TODO: Constructor
};

