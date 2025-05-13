#include <iostream>
#include <string>

using namespace std;
int main() {
  string sss("hello world !!!");
  cout << "capacity: " << sss.capacity() << " size: " << sss.size() << "\n";
  cout << sss.data() << "\n";
  return 0;
}
