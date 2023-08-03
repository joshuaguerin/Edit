
#include "interpreter.h"
#include <iostream>

using namespace std;

int main() {
  interpreter_t test;


  if(test.is_operator("+"))
    cout << 1 << endl;
  if(test.is_operator("-"))
    cout << 2 << endl;
  if(test.is_operator("*"))
    cout << 3 << endl;
  if(test.is_operator("/"))
    cout << 4 << endl;
  if(test.is_operator("**"))
    cout << 5 << endl;
  if(test.is_operator("+++"))
    cout << 6 << endl;
  if(test.is_operator("***"))
    cout << 7 << endl;

  // if(test.is_number("3948"))
  //   cout << 1 << endl;
  // if(test.is_number(".1"))
  //   cout << 2 << endl;
  // if(test.is_number("5."))
  //   cout << 3 << endl;
  // if(test.is_number("2.3"))
  //   cout << 4 << endl;
  
  
  // if(test.is_arithmetic("3948"))
  //   cout << 1 << endl;
  // if(test.is_arithmetic("valid_identifier"))
  //   cout << 2 << endl;
  // if(test.is_arithmetic("3_invalid"))
  //   cout << 3 << endl;
  
  
  return 0;
}
