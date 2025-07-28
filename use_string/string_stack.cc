#include <iostream>
#include <memory_resource>
#include <string>

#include "timer.h"

const int SIZE = 1000000;
void test_stack() {
  ScopedTimer timer("stack");
  for (int i = 0; i < SIZE; i++) {
    char buf[12];
    char buf1[12];
  }
}

void test_string_short() {
  ScopedTimer timer("std string");
  for (int i = 0; i < SIZE; i++) {
    std::string str("hello ");
    std::string str1("world!");
  }
}

void test_string_long() {
  ScopedTimer timer("std string");
  for (int i = 0; i < SIZE; i++) {
    std::string str("hello world, it is test string.");
  }
}

void test_pmr_string() {
  ScopedTimer timer("pmr string");
  for (int i = 0; i < SIZE; i++) {
    char buf[1024];
    std::pmr::monotonic_buffer_resource resource{std::data(buf), std::size(buf)};
    std::pmr::string s{&resource};

    s.append("it is a test");
    s.append(
        "it is a long string test;it is a long string test;it is a long string "
        "test;");
  }
}

void test_std_string() {
  ScopedTimer timer("std string 2");
  for (int i = 0; i < SIZE; i++) {
    std::string s;
    s.reserve(1024);
    s.append("it is a test");
    s.append(
        "it is a long string test;it is a long string test;it is a long string "
        "test;");
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
