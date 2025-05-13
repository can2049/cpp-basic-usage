#include <memory>

#include "log.h"
#include "object.h"

class Obj : public Object {
 public:
  Obj() = default;
  Obj(std::string s) : Object(s) { LOG(INFO) << __func__ << " obj start"; };
  ~Obj() { LOG(INFO) << __func__ << " obj dtor"; }
  int type = 0;
};

void test_default() {
  Obj obj;
  LOG(INFO) << "object name: " << obj.name;
}

void test_object() {
  LOG(INFO) << " === test object ===";
  Obj str("hhh");
  str.type = 2;

  LOG(INFO) << "str1. id: " << str.id() << " name: " << str.name
            << " type: " << str.type;
  auto str2 = str;
  LOG(INFO) << "str2. id: " << str2.id() << " name: " << str2.name
            << " type: " << str2.type;
}

void test_pointer() {
  LOG(INFO) << " === test pointer ===";
  std::unique_ptr<Object> a = std::make_unique<Obj>("derived");
}

int main() {
  init_log();
  test_default();
  test_object();
  test_pointer();
}
