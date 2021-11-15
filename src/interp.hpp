# pragma once

# include <string>
# include <variant>
# include <vector>

# include "token.hpp"
# include "expr.hpp"
# include "stmt.hpp"
# include "env.hpp"

struct Interpreter {
  Env environment;

  void interpret(std::vector<Stmt>);

  LiteralVal operator()(Binary);
  LiteralVal operator()(Grouping);
  LiteralVal operator()(Literal);
  LiteralVal operator()(Unary);
  LiteralVal operator()(Variable);
  LiteralVal operator()(Assignment);
  LiteralVal operator()(std::monostate);

  LiteralVal eval(Expr);
  void operator()(PrintStmt);
  void operator()(ExpressionStmt);
  void operator()(VarStmt);

  // ExLox
  void operator()(LocalsStmt);
  
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

