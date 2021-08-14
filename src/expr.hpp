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
> Expr;

struct Binary {
  Expr left;
  Token op;
  Exor right;

struct Grouping {
  Expr expression;
}

struct Literal {
  LiteralVal value;
}

struct Unary {
  Token op;
  Expr left; 
}
