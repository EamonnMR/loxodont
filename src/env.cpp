#include "env.hpp"
#include "interp.hpp"
#include "token.hpp"
#include <stdexcept>
#include <iostream>

void Env::define(std::string name, LiteralVal val){
  values.insert_or_assign(name, val);
}

LiteralVal Env::get(std::string name){
  if(values.count(name) == 0){
    throw std::runtime_error {"Undefined variable: " + name};
  } else {
    return values[name];
  }
}

void Env::printLocals(){
  std::cout << "Locals: \n";
  for (auto& kv: values) {
    std::cout << kv.first << ": " << repr(kv.second) << "\n";
  }
}
