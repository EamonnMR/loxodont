# import <vector>
# import "token.hpp"
# import "parser.hpp"

Parser::Parser(std::vector<Token> tokens){
  current = 0;
  this->tokens = tokens;
  // TODO: Heap of variants maybe?
  heapExpr = std::vector<Expr*>{};
  heapToken = std::vector<Token*>{};
}


Expr Parser::expression(){
  return equality();
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

/* Manual memory mgmt
 * Note that these are deleted in the destructor
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

Parser::~Parser(){
  for(auto expr : heapExpr){
    delete expr;
  }
  for(auto token : heapToken){
    delete token;
  }
}

