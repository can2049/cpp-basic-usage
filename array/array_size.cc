#include <signal.h>

#include <cassert>
#include <iostream>
#include <vector>

int main() {
  // Works with containers
  std::vector<int> v{3, 1, 4};
  assert(std::size(v) == 3);

  // And works with built-in arrays too
  int a[]{-5, 10, 15};
  // Returns the number of elements (not bytes) as opposed to sizeof
  assert(std::size(a) == 3);

  const struct {
    int number;
    const char* name;
  } kFailureSignals[] = {
      {SIGSEGV, "SIGSEGV"}, {SIGILL, "SIGILL"}, {SIGFPE, "SIGFPE"},
      {SIGABRT, "SIGABRT"}, {SIGBUS, "SIGBUS"},
  };

  std::cout << " size of struct: " << std::size(kFailureSignals) << "\n";
  std::cout << " data of struct: " << std::data(kFailureSignals) << "\n";

  std::cout << " struct empty?: " << std::empty(kFailureSignals) << "\n";

  int arr[] = {1};  // no content, cannot compile
  std::cout << " empty array?: " << std::size(arr) << "\n";
}
