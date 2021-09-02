std::string Interpreter::visit(Expr e){
  return std::visit(*this, e);
}

