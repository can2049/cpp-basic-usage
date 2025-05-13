#include <iostream>
using namespace std;

class Data {
 public:
  //   Data(int a = 4) : age(a) {}

  const int age{3};
};

int main() {
  //   Data a1(5);
  //   cout << a1.age << endl;
  Data a2;
  cout << a2.age << endl;
}