# pragma once

#include <variant>
#include <string>
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
  Expr * expr;
};

struct Literal {
  LiteralVal * value;
};

struct Unary {
  Token* op;
  Expr* right; 
};

struct ASTPrinter{
  std::string operator()(Binary);
  std::string operator()(Grouping);
  std::string operator()(Literal);
  std::string operator()(Unary);
  std::string parenthesize(std::string, std::vector<Expr>);
  std::string visit(Expr);
};

