#pragma once
#include <unordered_map>
#include <string>
#include "token.hpp"

struct Env {
  std::unordered_map<std::string, LiteralVal> values;
  void define(std::string, LiteralVal);
  LiteralVal get(std::string);
  void printLocals();
};

