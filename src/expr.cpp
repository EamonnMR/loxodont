#import <variant>

#import "expr.hpp"

void ASTPrinter::operator()(Binary b){}
void ASTPrinter::operator()(Grouping g){}
void ASTPrinter::operator()(Literal l){}
void ASTPrinter::operator()(Unary u){}

void visit_expr(Expr e){
  std::visit(ASTPrinter{}, e);
}
