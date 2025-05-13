#include <chrono>
#include <thread>

#include "object.h"

// using std::chrono_literals::se;

class Single : Object {
 public:
  static Single& instance() {
    static Single s;
    return s;
  }

  void hi() { LOG(INFO) << "hi"; }
  void loop() {
    while (!stop) {
      std::this_thread::sleep_for(1s);
    }
  }

 private:
  bool stop = false;
  Single() = default;
};

int main() {
  Single::instance().hi();
  Single::instance().loop();
}
