#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include <vector>

#include "token.hpp"

std::string repr(LiteralVal l){
  /**
   * Get the string representation of a literal.
   */
  if(l.has_value()){
    std::string litstr {""};
    NonNullLiteralVal val {l.value()};
    std::visit([&litstr](const auto &elem) mutable {
      std::stringstream str {};
      str << elem;
      litstr += str.str(); 
    }, val);

    return litstr;
  } else {
    return std::string {"null"};
  }
}


std::string Token::toString(){
  return std::string {tokenTypeStrings[type] + ": " + lexeme + " " + repr(literal)};
}

