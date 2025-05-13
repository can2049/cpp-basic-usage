#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

constexpr char kA[] = "aaa";
constexpr char kB[] = "bbb";
constexpr char kC[] = "ccc";

int main() {
  unordered_map<string_view, vector<int>> m;

  {
    std::string aa = std::string("aa") + 'a';

    cout << "aa -> " << aa << endl;

    cout << "p of kA: " << &kA << endl;
    cout << "p of aa: " << (void*)aa.data() << endl;

    m[aa].push_back(333);

    // key changed
    aa = "new_string";
  }

  m[kA].push_back(3);
  m[kB].push_back(4);
  m[kC].push_back(5);

  for (auto& [k, v] : m) {
    cout << k << " -> ";
    for (auto i : v) {
      cout << i << ", ";
    }
    cout << endl;
  }
}
