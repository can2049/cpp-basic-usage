#include <chrono>
#include <iostream>
#include <thread>
using namespace std;

void func() {
  cout << " begin func.  tid:" << std::this_thread::get_id() << endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));
  cout << " end func  tid:" << std::this_thread::get_id() << endl;
}

int main() {
  cout << "begin main   tid:" << std::this_thread::get_id() << endl;
  thread thr(func);
  thr.detach();  // detach之后，线程独立运行，主线程结束，线程结束
  cout << "end main   tid:" << std::this_thread::get_id() << endl;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  return 0;
}
