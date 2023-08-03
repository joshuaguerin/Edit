
#include <iostream>
#include <string>
#include <regex>

class interpreter_t{
 public:
  interpreter_t();
  bool is_arithmetic(std::string expression);
  bool is_identifier(std::string ident);
  bool is_number(std::string number);
  bool is_operator(std::string op);
  bool is_reserved(std::string word);
};
