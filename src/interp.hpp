# pragma once

# include <string>
# include <variant>
# include <vector>

# include "token.hpp"
# include "expr.hpp"
# include "stmt.hpp"

struct Interpreter {

  void interpret(std::vector<Stmt>);

  LiteralVal operator()(Binary);
  LiteralVal operator()(Grouping);
  LiteralVal operator()(Literal);
  LiteralVal operator()(Unary);
  LiteralVal operator()(std::monostate);

  LiteralVal eval(Expr);
  void operator()(PrintStmt);
  void operator()(ExpressionStmt);
  
  void eval(Stmt);

  bool isTruthy(LiteralVal);
  bool isEqual(LiteralVal, LiteralVal);
  long double numCast(LiteralVal);
  std::string strCast(LiteralVal);

  void checkNumOperands(Token, std::vector<LiteralVal>);

};

struct LoxRuntimeError : public std::runtime_error {
  Token token;
  std::string message;
  LoxRuntimeError(Token, std::string);
  const char* what() const noexcept;
};

