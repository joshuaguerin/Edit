#include <deque>
#include <string>


class buffer {
 private:
  std::deque<std::string> text;
 public:
  std::string& operator [](unsigned int i);
  
  void erase(int position);
  void insert(int position, std::string line);
  void push_back(std::string line);
  unsigned int size();
};
