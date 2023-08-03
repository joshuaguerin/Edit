#include <iostream>
#include <string>
#include <sstream>
#include <stack>

#include "converter.h"

using namespace std;

postfix_converter::postfix_converter(std::string infix) {
  expression = infix;
  preProcessExpression();
}

void postfix_converter::set(std::string infix) {
  expression = infix;
  preProcessExpression();
}

string postfix_converter::postfix() {
  stringstream e(expression), output;
  string current;
  stack<char> s;
  
  while(e >> current) {
    //    e >> current;
    //    cout << "current: " << current << endl;
    
    
    //1. Operand
    if(isOperand(current)) {
      output << current << " ";
    }
    //2. Operator, empty stack or top of stack is "("
    else if(isOperator(current) && (s.empty() || s.top() == '(')) {
      if(current != "**")
	s.push(current[0]);
      else
	s.push('^');
    }
    //3. Left parenthesis
    else if(current == "(") {
      s.push(current[0]);
    }
    //4. Right parenthesis
    else if(current == ")") {
      while(s.top() != '(') {
	output << s.top() << " ";
	s.pop();
      }
      //Pop left parenthesis
      s.pop();
    }
    //5. Operator
    else if(isOperator(current) && !s.empty()) {
      
      //Handle ** vs. *
      char curr = current[0];
      if(current == "**")
	curr = '^';
      
      while(!s.empty() && hasPriority(s.top(), curr)) {
	output << s.top() << " ";
	s.pop();
      }
      if(current != "**")
	s.push(current[0]);
      else
	s.push('^');
    }
    //    cout << output.str() << endl;
  }
  
  //6. Clear stack
  while(!s.empty()) {
    output << s.top() << " ";
    s.pop();
  }
  
  return output.str();
}


bool postfix_converter::isOperand(const string str) {
  for(int i=0; i<str.length(); i++) {
    switch (str[i]) {
      //Check for all characters outside of a-z, A-Z, 0-9, _
    case 0 ... 45:
    case 47:
    case 58 ... 64:
    case 91 ... 94:
    case 96:
    case 123 ... 127:
      return false;
    }
  }
  return true;
}


bool postfix_converter::isOperator(const string str) {
  return str == "+" || str == "-" || str == "*" || str == "**" || str == "/";
}

bool postfix_converter::hasPriority(char top, char current) {
  int top_priority=0, current_priority=0;

  if(top == '^')
    top_priority = 3;
  if(top == '*' || top == '/')
    top_priority = 2;
  else if(top == '+' || top == '-')
    top_priority = 1;
  
  if(current == '^')
    current_priority = 3;
  if(current == '*' || current == '/')
    current_priority = 2;
  else if(current == '+' || current == '-')
    current_priority = 1;

  return current_priority <= top_priority;
}


void postfix_converter::preProcessExpression() {
  int i=0;
  
  while(i<expression.size()) {
    // Most arithmetic operators and grouping symbols
    if(expression[i] == '+' ||
       expression[i] == '-' ||
       expression[i] == '/' ||
       expression[i] == '(' ||
       expression[i] == ')') {
      
      //Pad arithmetic operator or grouping symbol
      expression.insert(i, 1, ' ');
      i += 2;
      expression.insert(i, 1, ' ');
    }
    //Differentiate between * and **
    else if(expression[i] == '*') {
      if(expression[i-1] != '*' && expression[i+1] != '*') {
	expression.insert(i, 1, ' ');
	i += 2;
	expression.insert(i, 1, ' ');
      }
      else if(expression[i+1] == '*') {
	expression.insert(i, 1, ' ');
	i += 3;
	expression.insert(i, 1, ' ');
      }
      
      
    }
    
    //cout << expression << endl;
    i++;
  }
  
}


string postfix_converter::getExpression() {
  return expression;
}
