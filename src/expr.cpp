# include <variant>
# include <string>
# include <cstdint>
# include <cstdarg>
# include <vector>
# include <string>

# include  "expr.hpp"

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
  return parenthesize("binary", {});
}

std::string ASTPrinter::operator()(Grouping g){
  return parenthesize("bla", {});
}

std::string ASTPrinter::operator()(Literal l){
  return parenthesize("literal", {});
}

std::string ASTPrinter::operator()(Unary u){
  return parenthesize(u.op->lexeme, {*u.right});
}

std::string ASTPrinter::visit(Expr e){
  return std::visit(e, *this);
}

