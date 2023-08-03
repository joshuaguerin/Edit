
#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib> //atof
#include <exception>

#include "evaluate.h"

using namespace std;

postfix_evaluate::postfix_evaluate(string postfix) {
  expression = postfix;
}


float postfix_evaluate::value(const unordered_map<string, float> &vars) {
  stringstream e(expression);
  stack<string> s;
  string current;
  double rhs, lhs, result;
  
  while(e >> current) {
    //cout << "Processing " << current << endl;
    
    if(isOperator(current)) {
      try {
	rhs = stod(s.top());
      }
      catch (exception& e) {
	// Get value if it exists
	unordered_map<string, float>::const_iterator it = vars.find(s.top());
	if (it != vars.end())
	  rhs = it->second;
	// Do not add variables to table if they don't
	else
	  throw std::invalid_argument("NameError: name '" + s.top() + 
				      "' is not defined");
	// Throw exception here?
      }
      
      s.pop();
      
      
      try {
	lhs = stod(s.top());
      }
      catch (exception& e) {
	// Get value if it exists
	unordered_map<string, float>::const_iterator it = vars.find(s.top());
	if (it != vars.end())
	  lhs = it->second; 
	// Do not add variables to table if they don't
	else
	  throw std::invalid_argument("NameError: name '" + s.top() + 
				      "' is not defined");
	// Throw exception here?
      }
      
      s.pop();
      
      if(current == "+")
	result = lhs + rhs;
      else if(current == "-")
	result = lhs - rhs;
      else if(current == "*")
	result = lhs * rhs;
      else if(current == "/") {
	if(rhs == 0)
	  throw std::domain_error("ZeroDivisionError: division by zero");
	result = lhs / rhs;
      }
      else if(current == "^")
	result = pow(lhs, rhs);
      s.push(to_string(result));
    }
    else {
      s.push(current);
    }
  }
  
  return stod(s.top());
}


bool postfix_evaluate::isOperator(string token) {
  if(token == "+" ||
     token == "-" ||
     token == "*" ||
     token == "/" ||
     token == "^")
    return true;
  
  return false;
}
