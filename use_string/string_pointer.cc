#include <assert.h>
#include <stdint.h>

#include <cstdint>
#include <string>

#include "log.h"
int main() {
  char data = 3;
  char* p = &data;

  LOG(INFO) << " pointer: " << (void*)p;

  auto p_data = reinterpret_cast<intptr_t>(p);
  LOG(INFO) << " p-data: " << p_data;

  auto p_s = std::to_string(p_data);
  LOG(INFO) << " p-s: " << p_s;

  // send

  auto p2_i = std::stoll(p_s);
  LOG(INFO) << " p2_i: " << p2_i;

  auto p2 = (char*)p2_i;
  LOG(INFO) << " p2 : " << (void*)p2;

  char* a = nullptr;
  LOG(INFO) << "nullptr: " << a;

  return 0;
}
