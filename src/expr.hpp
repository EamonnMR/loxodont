# pragma once

#include <variant>
#include <string>

#include "token.hpp"
struct Binary;
struct Grouping;
struct Literal;
struct Unary;
struct Variable;
struct Assignment;

typedef std::variant<
  std::monostate,
  Binary,
  Grouping,
  Literal,
  Unary,
  Variable,
  Assignment,
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

struct Variable {
  Token name;
};

struct Assignment {
  Token name;
  Expr* value;
};

struct ASTPrinter{
  std::string operator()(Binary);
  std::string operator()(Grouping);
  std::string operator()(Literal);
  std::string operator()(Unary);
  std::string operator()(Variable);
  std::string operator()(Assignment);
  std::string operator()(std::monostate);
  std::string parenthesize(std::string, std::vector<Expr>);
  std::string visit(Expr);
};

