#pragma once
#include <string>
#include <vector>
#include <array>
#include <variant>
#include <optional>
#include <unordered_map>

typedef std::variant<bool, int64_t, long double, std::string> NonNullLiteral;
typedef std::optional<NonNullLiteral> Literal;
std::string repr(Literal l);

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
  void addToken(TokenType, Literal);
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

