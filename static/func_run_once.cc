#include <iostream>

using namespace std;

void do_some_thing_once() { cout << " do some thing once" << endl; }

void do_some_thing2(int i) { cout << " do some thing: " << i << endl; }

void special_call_once_func(int i) {
  [[maybe_unused]] const static bool inited_var = []() {
    do_some_thing_once();
    return true;
  }();
  do_some_thing2(i);
}

int main() {
  for (int i = 0; i < 5; i++) {
    special_call_once_func(i);
  }
}
