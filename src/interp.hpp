# pragma once

# include <string>
# include <variant>

# include "token.hpp"

struct Interpreter {
  LiteralVal operator()(Binary);
  LiteralVal operator()(Grouping);
  LiteralVal operator()(Literal);
  LiteralVal operator()(Unary);
  LiteralVal operator()(std::monostate);
  LiteralVal eval(Expr);
  bool isTruthy(LiteralVal);
  long double numCast(LiteralVal);
  std::string strCast(LiteralVal);
};

