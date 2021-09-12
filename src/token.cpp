#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <typeinfo>

#include "token.hpp"

std::ostream& operator<<(std::ostream& os, const NullVal& dt){
  os << "Nil";
  return os;
}

std::string repr(LiteralVal val){
  /**
   * Get the string representation of a literal.
   */
  std::string litstr {""};
  std::visit([&litstr](const auto &elem) mutable {
    std::stringstream str {};
    str << elem;
    litstr += str.str(); 
}, val);

  return litstr;
}


std::string Token::toString(){
  return std::string {tokenTypeStrings[type] + ": " + lexeme + " " + repr(literal)};
}

