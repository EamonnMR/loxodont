# pragma once

# include "token.hpp"
# include <vector>

struct Parser {
  std::vector<Token> tokens;
  size_t current;
  Parser(std::vector<Token>);
};

