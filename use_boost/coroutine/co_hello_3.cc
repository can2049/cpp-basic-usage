#include <boost/coroutine2/all.hpp>
#include <iostream>
#include <string>
#include <tuple>

using boost::coroutines2::coroutine;

void cooperative(coroutine<std::tuple<int, std::string>>::pull_type &source) {
  auto args = source.get();
  std::cout << std::get<0>(args) << " " << std::get<1>(args) << '\n';
  source();
  args = source.get();
  std::cout << std::get<0>(args) << " " << std::get<1>(args) << '\n';
}

int main() {
  coroutine<std::tuple<int, std::string>>::push_type sink{cooperative};
  sink(std::make_tuple(0, "aaa"));
  sink(std::make_tuple(1, "bbb"));
  std::cout << "end\n";
}
