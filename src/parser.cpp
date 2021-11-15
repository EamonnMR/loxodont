# include <vector>
# include <optional>
# include <iostream>
# include <stdexcept>

# include "token.hpp"
# include "parser.hpp"

LiteralVal LIT_TRUE {true};
LiteralVal LIT_FALSE {false};
LiteralVal LIT_NIL {};

LiteralVal * LIT_TRUE_PTR {&LIT_TRUE};
LiteralVal * LIT_FALSE_PTR {&LIT_FALSE};
LiteralVal * LIT_NIL_PTR {&LIT_FALSE};


Parser::Parser(std::vector<Token> tokens){
  current = 0;
  this->tokens = tokens;
  // TODO: Heap of variants maybe?
  heapExpr = std::vector<Expr*>{};
  heapToken = std::vector<Token*>{};
  heapLiteral = std::vector<LiteralVal*>{};
}

std::vector<Stmt> Parser::parse(){
  std::vector<Stmt> stmts {};
  while(!isAtEnd()){
    stmts.push_back(declaration());
  }

  return stmts;
}

/*
 * Grammar rules
 */

Expr Parser::expression(){
  return assignment();
  // return equality();
}

Expr Parser::assignment(){
  Expr expr = equality();
  if(match({EQUAL})){
    Token equals = previous();
    Expr value = assignment()  // a = b = c = 1

    if(std::holds_alternative<Variable>(equals)){
      Token name = std::get<Variable>(equals).name;
      return Assignment{name, alloc(value)};
    }
    throw parseError(equals, "Invalid assignment target");
}

Expr Parser::equality(){
  Expr expr {comparison()};
  while(match({BANG_EQUAL, EQUAL_EQUAL})){
    Token op {previous()};
    Expr right = comparison();
    expr = Binary{alloc(expr), alloc(op), alloc(right)};
  }
  return expr;
}

Expr Parser::comparison(){
  Expr expr { term() };
  while(match({
        GREATER,
        GREATER_EQUAL,
        LESS,
        LESS_EQUAL
  })){
    Token op { previous() };
    Expr right { term() };
    expr = Binary{alloc(expr), alloc(op), alloc(right)};
  }
  return expr;
}

Expr Parser::term() {
  Expr expr { factor() };

  while (match({MINUS, PLUS})){
    Token op { previous() };
    Expr right { factor() };
    expr = Binary{alloc(expr), alloc(op), alloc(right)};
  }
  return expr;
}

Expr Parser::factor() {
  Expr expr { unary() };
  while (match({ SLASH, STAR })){
    Token op = previous();
    Expr right = unary();
    expr = Binary {alloc(expr), alloc(op), alloc(right)};
  }
  return expr;
}

Expr Parser::unary(){
  if(match({ BANG, MINUS })){
    Token op { previous() };
    Expr right { unary() };
    return Unary{alloc(op), alloc(right)};
  }

  return primary();
}

Expr Parser::primary() {
  if (match({TRUE})) return Literal {LIT_TRUE_PTR};
  if (match({FALSE})) return Literal {LIT_FALSE_PTR};
  if (match({NIL})) return Literal {LIT_NIL_PTR};

  if (match({NUMBER, STRING})) {
    return Literal { alloc(previous().literal) };
  }

  if (match({IDENTIFIER})){
    return Variable {previous()};
  }

  if (match({LEFT_PAREN})){
    Expr expr { expression() };
    consume(RIGHT_PAREN, "Expect ')' after expression");
    return Grouping { alloc(expr) };
  }
  
  throw parseError(peek(), "Expected Expression");
}

Stmt Parser::declaration(){
  try {
    if( match({VAR})) return varDeclaration();
    return statement();
  } catch (std::runtime_error) {
    synchronize();
    return emptyStmt();
  }
}

Stmt Parser::statement (){
  if(match({PRINT})) return printStmt();
  if(match({LOCALS})) return localsStmt();

  return expressionStmt();
}

Stmt Parser::printStmt(){
  Expr value = expression();
  consume(SEMICOLON, "Expected semicolon at end of print");
  return PrintStmt {value};
}

Stmt Parser::localsStmt(){
  consume(SEMICOLON, "Expected semicolon at end of locals");
  return LocalsStmt {};
}

Stmt Parser::expressionStmt(){
  Expr expr = expression();
  consume(SEMICOLON, "Expected ';' at end of expr");
  return ExpressionStmt {expr};
}

Stmt Parser::varDeclaration(){
  Token name {consume(IDENTIFIER, "Expected identifier")};
  Expr initializer {};
  if( match({EQUAL}) ){
    initializer = expression();
  }
  consume(SEMICOLON, "Expected ';' at end of var stmt");
  return VarStmt {name, initializer};
}


Stmt Parser::emptyStmt(){
  // This is close enough to a null expr...
  // Can't overload two different monostates on the
  // interpreter class!
  // TODO: If this is a pain, inherit from monostate
  // make a NullStmt class and use that instead.
  ExpressionStmt empty {Expr {}};
  return empty;
}

/*
 * Utilities
 */

bool Parser::match(std::vector<TokenType> types){
  for( auto type : types){
    if(check(type)){
      advance();
      return true;
    }
  }
  return false;
}

bool Parser::check(TokenType type){
  if (isAtEnd()) return false;
  return peek().type == type;
}

Token Parser::advance() {
  if(!isAtEnd()) current ++;
  return previous();
}

bool Parser::isAtEnd(){
  return peek().type == KW_EOF;
}

Token Parser::peek(){
  return tokens[current];
}

Token Parser::previous(){
  return tokens[current - 1];
}

Token Parser::consume(TokenType type, std::string message){
  if (check( type )) return advance();
  else throw parseError(peek(), message);
}

std::runtime_error Parser::parseError(Token token, std::string message){
  error(token.line, message);
  return std::runtime_error {message};
}

void Parser::synchronize(){
  advance();
  while( !isAtEnd() ){
    if( previous().type == SEMICOLON ) return;

    // I'm not sold on this fallthrough construction
    switch (peek().type) {
      case KW_CLASS:
      case FUN:
      case VAR:
      case KW_FOR:
      case KW_IF:
      case WHILE:
      case PRINT:
      case RETURN:
        return;
      default: ;
    }
    advance();
  }
}

/* 
 * Manual memory mgmt (not in the book)
 * Note that these are deleted in the destructor
 * Convention is to allocate only when adding to a tree
 * otherwise these types are passed by value
 */
// Todo: Malloc more sparingly

Expr* Parser::alloc(Expr expr){
  Expr* xpr = new Expr{expr};
  heapExpr.push_back(xpr);
  return xpr;
}

Token* Parser::alloc(Token token){
  Token* tkn = new Token{token};
  heapToken.push_back(tkn);
  return tkn;
}

LiteralVal* Parser::alloc(LiteralVal literal){
  LiteralVal* lit = new LiteralVal{literal};
  heapLiteral.push_back(lit);
  return lit;
}

Parser::~Parser(){
  for(auto expr : heapExpr){
    delete expr;
  }
  for(auto token : heapToken){
    delete token;
  }
  for(auto literal : heapLiteral){
    delete literal;
  }
}

