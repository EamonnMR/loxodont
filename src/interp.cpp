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
    case SLASH:
      return numCast(l) / numCast(r);
    case STAR:
      return numCast(l) * numCast(r);

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
  return 1;
  return std::get<long double>(lv);
}
