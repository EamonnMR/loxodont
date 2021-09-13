# include <variant>

# include "token.hpp"
# include "expr.hpp"
# include "interp.hpp"

LiteralVal Interpreter::eval(Expr e){
  return std::visit(*this, e);
}

LiteralVal Interpreter::operator()(Literal l){
  return *l.value;
}

LiteralVal Interpreter::operator()(Grouping g){
  return eval(*g.expr);
}

LiteralVal Interpreter::operator()(Unary u){
  Expr right = *u.right;
  switch (u.op->type){
    case BANG:
      return ! isTruthy(eval(right));
    case MINUS:
      // TODO: What about ints?
      return -1 * numCast(eval(right));
  }
  // TODO: make Unreachable
  return LiteralVal{};
}

LiteralVal Interpreter::operator()(Binary b){
  LiteralVal l { eval(*b.left) };
  LiteralVal r { eval(*b.right) };

  switch(b.op->type){
    case MINUS:
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
      break;
    case SLASH:
      return numCast(l) / numCast(r);
    case STAR:
      return numCast(l) * numCast(r);

    case GREATER:
      return numCast(l) > numCast(r);
    case GREATER_EQUAL:
      return numCast(l) >= numCast(r);
    case LESS:
      return numCast(l) < numCast(r);
    case LESS_EQUAL:
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
