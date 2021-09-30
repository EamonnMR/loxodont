# pragma once

# include <variant>

# include "expr.hpp"

struct PrintStmt {
  Expr expr;
};

struct ExpressionStmt {
  Expr expr;
};

typedef std::variant<
  ExpressionStmt,
  PrintStmt
> Stmt;

struct StmtPrinter{
  std::string operator()(PrintStmt);
  std::string operator()(ExpressionStmt);
  std::string visit(Stmt);
};

