# include <variant>
# include <iostream>

# include "token.hpp"
# include "expr.hpp"
# include "interp.hpp"
# include "main.hpp"

void Interpreter::interpret(std::vector<Stmt> stmts){
  try {
    for (Stmt stmt: stmts){
      eval(stmt);
    }
  } catch (LoxRuntimeError e) {
    error(e.token.line, e.message);
  }
}

LiteralVal Interpreter::eval(Expr e){
  return std::visit(*this, e);
}

void Interpreter::eval(Stmt e){
  return std::visit(*this, e);
}

LiteralVal Interpreter::operator()(Literal l){
  return *l.value;
}

LiteralVal Interpreter::operator()(Grouping g){
  return eval(*g.expr);
}

LiteralVal Interpreter::operator()(Variable v){
  return environment.get(v.name.lexeme);
}

LiteralVal Interpreter::operator()(Unary u){
  Expr right = *u.right;
  LiteralVal r { eval(right) };
  switch (u.op->type){
    case BANG:
      return ! isTruthy(r);
    case MINUS:
      checkNumOperands(*u.op, {r});
      // TODO: What about ints?
      return -1 * numCast(r);
  }
  // TODO: make Unreachable
  return LiteralVal{};
}

LiteralVal Interpreter::operator()(Binary b){
  LiteralVal l { eval(*b.left) };
  LiteralVal r { eval(*b.right) };

  switch(b.op->type){
    case MINUS:
      checkNumOperands(*b.op, {l, r});
      return numCast(l) - numCast(r);
    case PLUS:
      if(
        std::holds_alternative<long double>(l)
        && std::holds_alternative<long double>(r)
      ){
        return numCast(l) + numCast(r);
      }
      if(
        std::holds_alternative<long double>(l)
        && std::holds_alternative<long double>(r)
      ){
        return strCast(l) + strCast(r);
      }
      throw LoxRuntimeError {
        *b.op,
        "Operand types must match (string + string or num + num)"
      };
    case SLASH:
      checkNumOperands(*b.op, {l, r});
      return numCast(l) / numCast(r);
    case STAR:
      checkNumOperands(*b.op, {l, r});
      return numCast(l) * numCast(r);

    case GREATER:
      checkNumOperands(*b.op, {l, r});
      return numCast(l) > numCast(r);
    case GREATER_EQUAL:
      checkNumOperands(*b.op, {l, r});
      return numCast(l) >= numCast(r);
    case LESS:
      checkNumOperands(*b.op, {l, r});
      return numCast(l) < numCast(r);
    case LESS_EQUAL:
      checkNumOperands(*b.op, {l, r});
      return numCast(l) <= numCast(r);

    case EQUAL_EQUAL:
      return isEqual(l, r);
    case BANG_EQUAL:
      return !isEqual(l, r);


  }
  return LiteralVal{};
}

LiteralVal Interpreter::operator()(std::monostate nil){
  return LiteralVal{};
}

bool Interpreter::isTruthy(LiteralVal l){
  if(std::holds_alternative<bool>(l)){
    return std::get<bool>(l);
  }
  if(std::holds_alternative<NullVal>(l)){
    return false;
  }
  return true;
}

long double Interpreter::numCast(LiteralVal lv){
  return std::get<long double>(lv);
}

std::string Interpreter::strCast(LiteralVal sv){
  return std::get<std::string>(sv);
}

bool Interpreter::isEqual(LiteralVal l, LiteralVal r){
  if(
    std::holds_alternative<NullVal>(l)
    && std::holds_alternative<NullVal>(r)
  ){
    return true;
  } else if (
      std::holds_alternative<NullVal>(l)
      || std::holds_alternative<NullVal>(r)
  ){
    return false;
  }
  // TODO: Return error if l.index() != r.index()
  // TODO: Is there a more elegant way to do this?
  if(std::holds_alternative<bool>(l)){
    return std::get<bool>(l) == std::get<bool>(r);
  }
  if(std::holds_alternative<long double>(l)){
    return numCast(l) == numCast(r);
  }
  if(std::holds_alternative<std::string>(l)){
    return strCast(l) == strCast(r);
  }

  //TODO: Handle more cases
  return false;

}

void Interpreter::operator()(ExpressionStmt stmt){
  eval(stmt.expr);
}

void Interpreter::operator()(PrintStmt stmt){
  // std::cout << ASTPrinter().visit(eval(stmt.expr));
  std::cout << repr(eval(stmt.expr)) << "\n";
}


void Interpreter::operator()(VarStmt stmt){
  std::cout << "Interpret Var Stmt";
  LiteralVal v;
  if (
      !std::holds_alternative<std::monostate>(
        stmt.initializer)
  ){

  std::cout << "Non-null initializer";
    v = eval(stmt.initializer);
  } else {
    std::cout << "Null initializer";
  }
  environment.define(stmt.name.lexeme, v);
}

void Interpreter::checkNumOperands(Token op, std::vector<LiteralVal> nums){
  for(LiteralVal l : nums){
    if(!std::holds_alternative<long double>(l)){
      throw LoxRuntimeError {
        op,
        "Operator requires numerics"
      };
    }
  }
}

LoxRuntimeError::LoxRuntimeError(Token token, std::string message) : std::runtime_error(message){
  this->token = token;
  this->message = message;
}

const char* LoxRuntimeError::what() const noexcept{
  return message.c_str();
}

