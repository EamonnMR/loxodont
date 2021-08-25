# pragma once

# include "token.hpp"
# include "expr.hpp"
# include <vector>

struct Parser {
  std::vector<Token> tokens;  // Input
  // Parser-allocated memory
  std::vector<Expr*> heapExpr;
  std::vector<Token*> heapToken;
  size_t current;

  Parser(std::vector<Token>);
  ~Parser();
  // Grammar Rules
  Expr expression();
  Expr equality();
  Expr comparison();

  // Utility
  bool match(std::vector<TokenType>);
  bool check(TokenType);
  Token advance();
  bool isAtEnd();
  Token peek();
  Token previous();

  // Utility (Not in the book)
  Expr * alloc(Expr);
  Token * alloc(Token);
};
