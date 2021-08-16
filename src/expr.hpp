# pragma once

#include <variant>
#include "token.hpp"
struct Binary;
struct Grouping;
struct Literal;
struct Unary;

typedef std::variant<
  Binary,
  Grouping,
  Literal,
  Unary
> Expr;

struct Binary {
  Expr *left;
  Token *op;
  Expr *right;
};

struct Grouping {
  Expr * expression;
};

struct Literal {
  LiteralVal * value;
};

struct Unary {
  Token* op;
  Expr* left; 
};

struct ASTPrinter{
  void operator()(Binary);
  void operator()(Grouping);
  void operator()(Literal);
  void operator()(Unary);
};
