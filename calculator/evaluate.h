
#ifndef EVALUATE_H
#define EVALUATE_H

#include <unordered_map>
#include <string>

class postfix_evaluate{
 private:
  std::string expression;
 public:
  postfix_evaluate(std::string postfix);
  float value(const std::unordered_map<std::string, float> &vars);
  /*
   * Note: vars is const reference so we don't accidentally add
   *       missing identifiers to the symbol table.
   */
  
  bool isOperator(std::string token);
};

#endif
