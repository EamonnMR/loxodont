# pragma once

# include <variant>

# include "expr.hpp"
# include "token.hpp"

struct PrintStmt {
  Expr expr;
};

struct ExpressionStmt {
  Expr expr;
};

struct VarStmt {
  Token name;
  Expr initializer;
};

typedef std::variant<
  ExpressionStmt,
  PrintStmt,
  VarStmt
> Stmt;

struct StmtPrinter{
  std::string operator()(PrintStmt);
  std::string operator()(ExpressionStmt);
  std::string operator()(VarStmt);
  std::string visit(Stmt);
};

