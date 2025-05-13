#include <boost/coroutine2/all.hpp>
#include <functional>
#include <iostream>

using boost::coroutines2::coroutine;

void cooperative(coroutine<int>::push_type &sink, int i) {
  int j = i;
  std::cout << __func__ << " start\n";
  sink(++j);
  sink(++j);
  std::cout << __func__ << " end\n";
}

int main() {
  using std::placeholders::_1;
  coroutine<int>::pull_type source{std::bind(cooperative, _1, 0)};
  std::cout << " call get: " << source.get() << '\n';
  source();
  std::cout << " call get: " << source.get() << '\n';
  source();
  std::cout << __func__ << " end\n";
}
