struct Interpreter{
  std::string operator()(Binary);
  std::string operator()(Grouping);
  std::string operator()(Literal);
  std::string operator()(Unary);
  std::string operator()(std::monostate);
  std::string visit(Expr);
};

