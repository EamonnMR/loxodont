# pragma once

# include <string>
# include <variant>
# include <vector>

# include "token.hpp"

struct Interpreter {
  LiteralVal operator()(Binary);
  LiteralVal operator()(Grouping);
  LiteralVal operator()(Literal);
  LiteralVal operator()(Unary);
  LiteralVal operator()(std::monostate);

  LiteralVal eval(Expr);

  bool isTruthy(LiteralVal);
  bool isEqual(LiteralVal, LiteralVal);
  long double numCast(LiteralVal);
  std::string strCast(LiteralVal);

  void checkNumOperands(Token, std::vector<LiteralVal>);

};

LoxRuntimeError::LoxRuntimeError(Token token, std::string message) : std::runtime_error(message){
  this->token = token;
  this->message = message;
};

const char* LoxRuntimeError::what() const noexcept{
  return message.c_str();
}
