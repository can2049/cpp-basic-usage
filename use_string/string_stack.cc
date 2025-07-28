#include <iostream>
#include <memory_resource>
#include <string>

#include "timer.h"

constexpr size_t SIZE = 10000;

void test_stack() {
  std::array<const char*, SIZE * 2> cache;
  cache.fill(nullptr);
  std::cout << "Testing stack buffer performance...\n";
  ScopedTimer timer("stack buffer");
  for (size_t i = 0; i < SIZE; i++) {
    char buf[12];
    char buf1[12];

    const char* p = std::data(buf);
    const char* p1 = std::data(buf1);

    cache[i * 2] = p;
    cache[i * 2 + 1] = p1;
  }
}

void test_string_short() {
  std::array<const char*, SIZE * 2> cache;
  cache.fill(nullptr);

  ScopedTimer timer("std string short");
  for (size_t i = 0; i < SIZE; i++) {
    std::string str("hello ");
    std::string str1("world!");

    const char* p = str.data();
    const char* p1 = str1.data();
    cache[i * 2] = p;
    cache[i * 2 + 1] = p1;
  }
}

void test_string_long() {
  std::array<const char*, SIZE * 2> cache;
  cache.fill(nullptr);
  ScopedTimer timer("std string long");
  for (size_t i = 0; i < SIZE; i++) {
    std::string str("hello world, it is test string.");
    const char* p = str.data();
    cache[i * 2] = p;
    cache[i * 2 + 1] = p;
  }
}

void test_pmr_string() {
  std::array<const char*, SIZE * 2> cache;
  cache.fill(nullptr);
  ScopedTimer timer("pmr string");
  for (size_t i = 0; i < SIZE; i++) {
    char buf[1024];
    std::pmr::monotonic_buffer_resource resource{std::data(buf),
                                                 std::size(buf)};
    std::pmr::string s{&resource};

    s.append("it is a test");
    s.append(
        "it is a long string test;it is a long string test;it is a long string "
        "test;");
    const char* p = s.data();
    cache[i * 2] = p;
    cache[i * 2 + 1] = p;
  }
}

void test_std_string() {
  std::array<const char*, SIZE * 2> cache;
  cache.fill(nullptr);
  ScopedTimer timer("std string");
  for (size_t i = 0; i < SIZE; i++) {
    std::string s;
    s.reserve(1024);
    s.append("it is a test");
    s.append(
        "it is a long string test;it is a long string test;it is a long string "
        "test;");
    const char* p = s.data();
    cache[i * 2] = p;
    cache[i * 2 + 1] = p;
  }
}

void test_string_stack() {
  std::cout << "Testing string stack performance...\n";
  test_stack();
  test_string_short();
  test_string_long();
}

void test_pmr_string_stack() {
  std::cout << "Testing pmr string stack performance...\n";
  test_pmr_string();
  test_std_string();
}

int main() {
  test_string_stack();
  test_pmr_string_stack();
  return 0;
}
