
#include "buffer.h"

using namespace std;


std::string& buffer::operator[](unsigned int i) {
  return text[i];
}

void buffer::erase(int position){
  text.erase(text.begin() + position);
}

void buffer::insert(int position, std::string line){
  text.insert(text.begin() + position, line);
}


void buffer::push_back(string line) {
  text.push_back(line);
}


unsigned int buffer::size() {
  return text.size();
}
