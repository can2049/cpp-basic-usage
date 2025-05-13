#include <iostream>
#include <memory>
#include <vector>

#include "log.h"
#include "object.h"
#include "timer.h"

struct Data {
  Object a;
};

int main() {
  init_log();
  Data d1;
  d1.a.name = "hello";
  LOG(INFO) << "----- begin to move -------d1 id: " << d1.a.id()
            << "----------";
  auto d2 = std::move(d1);
  LOG(INFO) << "----- after move -------d2 id: " << d2.a.id() << "----------";
}
