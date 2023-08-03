
#include <iostream>
#include "editor.h"

using namespace std;



int main(int argc,char *argv[]) {
  extern void perror(), exit();
  
  editor e;
  
  // File argument on command line
  if(argc == 2) {
    e.read_file(argv[1]);
  }

  e.main_loop();
  cout << "Exit successful." << endl;
  
  
  return 0;
}
