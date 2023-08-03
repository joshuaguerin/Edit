
#include <iostream>

#include "interpreter.h"


using namespace std;

interpreter_t::interpreter_t() {
  
}


bool interpreter_t::is_arithmetic(std::string expression) {
  string ident = "([a-zA-Z_][a-zA-Z_0-9]*)";
  string number = "[0-9]+.[0-9]*|[0-9]*.[0-9]+";
  
  // Match any number of +'s, -, /, **, *
  string op = "(\\++|-|/|\\*\\*|\\*)";
  // Note to self, behaves strange when I swap ** and *
  // Something must be encoded wrong?
  
  regex arithmetic_regex("");
  if(regex_match(expression, arithmetic_regex))
    return true;
  return false;
}


bool interpreter_t::is_identifier(string ident) {
  regex ident_regex("[a-zA-Z_][a-zA-Z_0-9]*");
  if(regex_match(ident, ident_regex) && !is_reserved(ident))
    return true;
  return false;
}

bool interpreter_t::is_number(string number) {
  regex number_regex("[0-9]+.[0-9]*|[0-9]*.[0-9]+");
  if(regex_match(number, number_regex))
    return true;
  return false;
}

bool interpreter_t::is_operator(string op) {
  regex operator_regex("(\\++|-|/|\\*\\*|\\*)");
  if(regex_match(op, operator_regex))
    return true;
  return false;
}

bool interpreter_t::is_reserved(string word) {
  if(word == "False" || word == "class" || word == "finally" ||
     word == "is" || word == "return" || word == "None" ||
     word == "continue" || word == "for" || word == "lambda" ||
     word == "try" || word == "True" || word == "def" ||
     word == "from" || word == "nonlocal" || word == "while" ||
     word == "and" || word == "del" || word == "global" ||
     word == "not" || word == "with" || word == "as" ||
     word == "elif" || word == "if" || word == "or" ||
     word == "yield" || word == "assert" || word == "else" ||
     word == "import" || word == "pass" || word == "break" ||
     word == "except" || word == "in" || word == "raise")
    return true;
  return false;
}
