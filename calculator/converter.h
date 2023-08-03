
#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>

class postfix_converter {
 private:
  std::string expression;
 public:
  postfix_converter(std::string infix);
  void set(std::string infix);
  std::string postfix();
  bool isOperand(const std::string str);
  bool isOperator(const std::string str);
  bool hasPriority(char top, char current);
  void preProcessExpression();
  std::string getExpression();
};

#endif
