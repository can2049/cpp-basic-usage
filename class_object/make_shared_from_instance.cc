#include <memory>

#include "class_object/object.h"
#include "log.h"

int main() {
  init_log();

  auto a = Object("custom string");
  LOG(INFO) << "-----";
  auto b = std::make_shared<Object>(a);
}
