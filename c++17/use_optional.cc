#include <iostream>
#include <optional>
using namespace std;
int main() {
  optional<int> v_i;
  cout << *v_i << endl;

  optional<float> v_f;
  cout << *v_f << endl;
}
