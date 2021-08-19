# include <variant>
# include <vector>
# include <string>

# include "expr.hpp"
# include "token.hpp"

std::string ASTPrinter::parenthesize(std::string name, std::vector<Expr> exprs ){
  std::string result { "(" + name};
  for(Expr arg : exprs){
    result += visit(arg);
    result += " ";
  }
  result += ")";
  return result;
}

std::string ASTPrinter::operator()(Binary b){
  return parenthesize(b.op->lexeme, {*b.left, *b.right});
}

std::string ASTPrinter::operator()(Grouping g){
  return parenthesize("group", {*g.expr});
}

std::string ASTPrinter::operator()(Literal l){
  return repr(*l.value);
}

std::string ASTPrinter::operator()(Unary u){
  return parenthesize(u.op->lexeme, {*u.right});
}

std::string ASTPrinter::visit(Expr e){
  return std::visit(*this, e);
}

