#include <boost/coroutine2/all.hpp>
#include <boost/version.hpp>
#include <iostream>
using namespace boost::coroutines2;

void cooperative(coroutine<void>::push_type &sink) {
  std::cout << __func__ << " Coroutine started\n";
  sink();  // yield and wait for resume
  std::cout << __func__ << " Resumed first time\n";
  sink();  // yeild again
  std::cout << __func__ << " Resumed second time, now ending\n";
}

int main() {
  std::cout << __func__ << " Boost Library Version: " << BOOST_LIB_VERSION
            << std::endl;
  std::cout << __func__ << " 0000000\n";
  // start coroutine, will call cooperative function, till first yield point
  coroutine<void>::pull_type source{cooperative};
  std::cout << __func__ << " 1111111\n";
  // start from first yield point
  source();
  std::cout << __func__ << " 2222222\n";
  // start from second yield point, till end
  source();
  std::cout << __func__ << " 3333333\n";
  // the function has ended, so no more output
  source();
  std::cout << __func__ << " 4444444\n";
}
