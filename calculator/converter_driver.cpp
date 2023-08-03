
#include <iostream>
#include "converter.h"
#include "evaluate.h"

using namespace std;

int main(int argc, char *argv[]) {
  unordered_map<string, float> vars;
  vars["x"] = 3;
  //vars["y"] = 2;
  
  if(argc == 1) {
    string postfix;
    //postfix_converter c("(3+4*2)/(1-5)*2+3**2");
    postfix_converter c("(x+4*y)/(1-5)*y+x**y");
    //postfix_converter c("9 / 8**1 * 4");
    //postfix_converter c("(x+3)-c");
    c.preProcessExpression();
    cout << c.getExpression() << endl;
    postfix = c.postfix();
    cout << postfix << endl;
    
    postfix_evaluate e(postfix);
    cout << e.value(vars) << endl;
    
    // postfix_converter c(argv[1]);
    // postfix = c.postfix();
    // cout << postfix << endl;
    // postfix_evaluate e(postfix);
    // cout << e.value() << endl;
  }
  else 
    cout << "Arithmetic argument required." << endl;
  
  return 0;
}
