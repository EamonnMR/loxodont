# pragma once

# include <vector>
# include <stdexcept>

# include "token.hpp"
# include "expr.hpp"
# include "main.hpp"

struct Parser {
  std::vector<Token> tokens;  // Input
  // Parser-allocated memory
  std::vector<Expr*> heapExpr;
  std::vector<Token*> heapToken;
  std::vector<LiteralVal*> heapLiteral;
  size_t current;

  Parser(std::vector<Token>);
  ~Parser();
  Expr parse();
  // Grammar Rules
  Expr expression();
  Expr equality();
  Expr comparison();
  Expr term();
  Expr factor();
  Expr unary();
  Expr primary();

  // Utility
  bool match(std::vector<TokenType>);
  bool check(TokenType);
  Token advance();
  bool isAtEnd();
  Token peek();
  Token previous();
  Token consume(TokenType, std::string);
  std::runtime_error parseError(Token, std::string);
  void synchronize();

  // Utility (Not in the book)
  Expr * alloc(Expr);
  Token * alloc(Token);
  LiteralVal * alloc(LiteralVal);
};
